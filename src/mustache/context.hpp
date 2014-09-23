// Copyright (c) 2014, Daniel Pfeifer <daniel@pfeifer-mail.de>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef MUSTACHE_CONTEXT_HPP
#define MUSTACHE_CONTEXT_HPP

#include <iosfwd>
#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include "engine.hpp"
#include "to_string.hpp"
#include "is_empty.hpp"
#include "get_element.hpp"

namespace mustache
{

class Context
{
public:
	Context() = default;

	template<typename T>
	Context(T const& x) :
			model(new Model<T>(x))
	{
	}

	Context get(std::string const &name) const
	{
		if (!model)
		{
			return Context();
		}

		Context tmp = model->get_(name);

		if (tmp)
		{
			tmp.parent = this;
		}
		else if (parent)
		{
			tmp = parent->get(name);
		}

		return tmp;
	}

	explicit operator bool() const
	{
		return bool(model);
	}

	bool empty() const
	{
		return !model || model->is_empty_();
	}

	std::string to_string() const
	{
		return model ? model->to_string_() : "";
	}

	Engine::Iter render(Renderer& renderer) const
	{
		return model->render_(renderer);
	}

private:
	struct Concept
	{
		virtual ~Concept() = default;
		virtual bool is_empty_() const = 0;
		virtual std::string to_string_() const = 0;
		virtual Engine::Iter render_(Renderer &renderer) const = 0;
		virtual Context get_(std::string const& name) const = 0;
	};

	template<typename T>
	struct Model: Concept
	{
		Model(T const &x) :
				ref(x)
		{
		}

		template<typename U, typename F>
		static Engine::Iter visit(U const& value, F&& function)
		{
			return function(value);
		}

		template<typename U, typename F>
		static Engine::Iter visit(boost::optional<U> const& value, F&& function)
		{
			assert(value);
			return function(*value);
		}

		template<typename U, typename F>
		static Engine::Iter visit(std::vector<U> const& value, F&& function)
		{
			Engine::Iter it;
			for (auto&& elem : value)
			{
				// TODO: iterate with index
				// TODO: create Context with idx and value
				// TODO: create Context from value, with idx+value as parent
				it = function(elem);
			}
			return it;
		}

		bool is_empty_() const override
		{
			return is_empty(ref);
		}

		std::string to_string_() const override
		{
			using std::to_string;
			using mustache::to_string;
			return to_string(ref);
		}

		Engine::Iter render_(Renderer& render) const override
		{
			visit(ref, [&render](Context const& v)
			{
				return render.render(v);
			});
		}

		Context get_(std::string const& name) const override
		{
			return get_element(ref, name);
		}

		T const& ref;
	};

private:
	Context const* parent = nullptr;
	std::unique_ptr<Concept const> model;
};

} // namespace mustache

#endif /* MUSTACHE_CONTEXT_HPP */
