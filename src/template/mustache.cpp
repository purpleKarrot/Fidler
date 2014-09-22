/*
 * Copyright (C) 2013 Daniel Pfeifer <daniel@pfeifer-mail.de>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#include "mustache.hpp"
#include <boost/regex.hpp>
#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>

namespace mustache
{

json::Value get_value(json::Object const& obj, std::string const& key)
{
  auto const it = obj.find(key);
  if (it != obj.end())
  {
    return it->second;
  }
  return nullptr;
}

class print_visitor: public boost::static_visitor<void>
{
public:
  print_visitor(std::ostream& out) :
      out(out)
  {
  }

  void operator()(std::nullptr_t) const
  {
  }

  void operator()(bool value) const
  {
    out << (value ? "true" : "false");
  }

  void operator()(double value) const
  {
    out << value;
  }

  void operator()(long long value) const
  {
    out << value;
  }

  void operator()(std::string const& value) const
  {
    out << value;
  }

  void operator()(json::Object const& value) const
  {
  }

  void operator()(json::Array const& value) const
  {
  }

protected:
  std::ostream& out;
};

class print_escaped_visitor: public print_visitor
{
public:
  using print_visitor::print_visitor;
  using print_visitor::operator();

  void operator()(std::string const& value) const
  {
    for (char c : value)
    {
      switch (c)
      {
      case '<':
        out << "&lt;";
        break;
      case '>':
        out << "&gt;";
        break;
      case '&':
        out << "&amp;";
        break;
      case '\"':
        out << "&quot;";
        break;
      default:
        out << c;
      }
    }
  }
};

static void escape(std::ostream& out, json::Value const& value)
{
  print_escaped_visitor visitor(out);
  boost::apply_visitor(visitor, value);
}

class section_visitor_base: public boost::static_visitor<Template::Iter>
{
public:
  section_visitor_base(Template const& self, Template::Iter begin,
      Template::Iter end, json::Object const& ctx, std::ostream& out) :
      self(self), begin(begin), end(end), out(out), ctx(ctx)
  {
  }

protected:
  Template::Iter ignore() const
  {
    std::ostream null{nullptr};
    return self.do_render(begin, end, {}, null);
  }

  Template::Iter render(json::Object const& obj) const
  {
    return self.do_render(begin, end, obj, out);
  }

private:
  Template const& self;
  Template::Iter begin;
  Template::Iter end;
  std::ostream& out;

protected:
  json::Object const& ctx;
};

struct section_visitor: section_visitor_base
{
  using section_visitor_base::section_visitor_base;

  Template::Iter operator()(std::nullptr_t) const
  {
    return ignore();
  }

  Template::Iter operator()(bool value) const
  {
    return value ? render(ctx) : ignore();
  }

  Template::Iter operator()(double value) const
  {
    return (value != 0.0) ? render(ctx) : ignore();
  }

  Template::Iter operator()(long long value) const
  {
    return (value != 0) ? render(ctx) : ignore();
  }

  Template::Iter operator()(std::string const& value) const
  {
    return (!value.empty()) ? render(ctx) : ignore();
  }

  // fusion sequence
  Template::Iter operator()(json::Object const& value) const
  {
    return (!value.empty()) ? render(value) : ignore();
  }

  // std::vector
  Template::Iter operator()(json::Array const& value) const
  {
    if (value.empty())
    {
      return ignore();
    }

    Template::Iter end;

    for (auto& elem : value)
    {
      end = render(boost::get<json::Object>(elem));
    }

    return end;
  }
};

struct inverted_visitor: section_visitor_base
{
  using section_visitor_base::section_visitor_base;

  Template::Iter operator()(std::nullptr_t) const
  {
    return render(ctx);
  }

  Template::Iter operator()(bool value) const
  {
    return !value ? render(ctx) : ignore();
  }

  Template::Iter operator()(double value) const
  {
    return (value == 0.0) ? render(ctx) : ignore();
  }

  Template::Iter operator()(long long value) const
  {
    return (value == 0) ? render(ctx) : ignore();
  }

  Template::Iter operator()(std::string const& value) const
  {
    return (value.empty()) ? render(ctx) : ignore();
  }

  Template::Iter operator()(json::Object const& value) const
  {
    return (value.empty()) ? render(ctx) : ignore();
  }

  Template::Iter operator()(json::Array const& value) const
  {
    return (value.empty()) ? render(ctx) : ignore();
  }
};

struct Template::Impl
{
  Impl(std::string path) :
      template_path(std::move(path)),
      tag_regex("\\{\\{(#|!|/|\\^|>)?(.+?)(\\|(.+?))?\\}\\}")
  {
    filters["escape"] = escape;
  }

  std::string template_path;
  boost::regex tag_regex;
  std::unordered_map<std::string, Filter> filters;
};

Template::Template(std::string path) :
    impl(new Impl(std::move(path)))
{
}

Template::~Template() = default;

std::string Template::render(const json::Object& ctx)
{
  std::ostringstream out;
  auto const tmp = load_template("main");
  do_render(tmp.begin(), tmp.end(), ctx, out);
  return out.str();
}

std::string Template::load_template(std::string const& name) const
{
  std::string result;
  std::ifstream file(impl->template_path + name + ".mustache");
  using iterator = std::istreambuf_iterator<char>;
  return std::string((iterator(file)), iterator());
}

Template::Iter Template::do_render(Iter begin, Iter end,
    json::Object const& ctx, std::ostream& out) const
{
  boost::match_results<Iter> matches;
  while (regex_search(begin, end, matches, impl->tag_regex))
  {
    std::copy(begin, matches[0].first, std::ostreambuf_iterator<char>(out));
    begin = matches[0].second;

    std::string const modifier(matches[1].first, matches[1].second);
    std::string const key(matches[2].first, matches[2].second);
    std::string const key2(matches[3].first, matches[3].second);
    std::string const key3(matches[4].first, matches[4].second);
    auto value = get_value(ctx, key);

    if (modifier == "!")
    {
      // ignore
    }
    else if (modifier == ">")
    {
      auto const partial = this->load_template(boost::get<std::string>(value));
      do_render(partial.begin(), partial.end(), ctx, out);
    }
    else if (modifier == "#")
    {
      section_visitor visitor(*this, begin, end, ctx, out);
      begin = boost::apply_visitor(visitor, value);
    }
    else if (modifier == "^")
    {
      inverted_visitor visitor(*this, begin, end, ctx, out);
      begin = boost::apply_visitor(visitor, value);
    }
    else if (modifier == "/") // TODO: check for matching begin/end
    {
      return begin;
    }
    else if (!key3.empty())
    {
      impl->filters.at(key3)(out, value);
    }
    else
    {
      print_visitor visitor(out);
      boost::apply_visitor(visitor, value);
    }
  }

  std::copy(begin, end, std::ostreambuf_iterator<char>(out));
  return end;
}

} // namespace mustache
