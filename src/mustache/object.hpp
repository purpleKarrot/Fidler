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

#ifndef MUSTACHE_OBJECT_HPP
#define MUSTACHE_OBJECT_HPP

#include <memory>
#include <functional>
#include <vector>
#include <boost/optional.hpp>
#include "to_string.hpp"
#include "is_empty.hpp"
#include "get_element.hpp"

namespace mustache
{

class Object
{
public:
	Object() = default;

	template<typename T>
	Object(T const& x) :
			model(new Model<T>(x))
	{
	}

	explicit operator bool() const
	{
		return bool(model);
	}

	Object find(std::string const& name) const
	{
		return model ? model->find_(name) : Object();
	}

	bool empty() const
	{
		return !model || model->is_empty_();
	}

	std::string to_string() const
	{
		return model ? model->to_string_() : "";
	}

	void for_each(std::function<void(Object)> function) const
	{
		if (model)
		{
			model->for_each_(function);
		}
	}

private:
	template<typename U, typename F>
	static void do_for_each(U const& value, F&& function)
	{
		return function(value);
	}

	template<typename U, typename F>
	static void do_for_each(boost::optional<U> const& value, F&& function)
	{
		assert(value);
		return function(*value);
	}

	template<typename U, typename F>
	static void do_for_each(std::vector<U> const& value, F&& function)
	{
		std::for_each(begin(value), end(value), function);
	}

	struct Concept
	{
		virtual ~Concept() = default;
		virtual bool is_empty_() const = 0;
		virtual std::string to_string_() const = 0;
		virtual Object find_(std::string const& name) const = 0;
		virtual void for_each_(std::function<void(Object)> function) const = 0;
	};

	template<typename T>
	struct Model: Concept
	{
		Model(T const &x) :
				ref(x)
		{
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

		Object find_(std::string const& name) const override
		{
			return get_element(ref, name);
		}

		void for_each_(std::function<void(Object)> function) const override
		{
			do_for_each(ref, function);
		}

		T const& ref;
	};

private:
	std::unique_ptr<Concept const> model;
};

} // namespace mustache

#endif /* MUSTACHE_OBJECT_HPP */
