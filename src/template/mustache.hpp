/*
 * Copyright (C) 2013 Daniel Pfeifer <daniel@pfeifer-mail.de>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef MUSTACHE_HPP
#define MUSTACHE_HPP

#include "json.hpp"
#include <iosfwd>
#include <memory>

namespace mustache
{

using Filter = std::function<void(std::ostream& out, json::Value const&)>;

class Template
{
public:
  explicit Template(std::string path);
  ~Template();

  std::string render(const json::Object& ctx);

  std::string load_template(std::string const& name) const;

  using Iter = std::string::const_iterator;
  Iter do_render(Iter begin, Iter end, json::Object const& ctx,
      std::ostream& out) const;


  void add_filter(std::string name, Filter filter);

private:
  struct Impl;
  std::unique_ptr<Impl> impl;
};

} // namespace mustache

#endif /* MUSTACHE_HPP */
