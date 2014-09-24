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

#include "object.hpp"
#include "engine.hpp"

namespace mustache
{

struct Tag
{
	std::string name;
	std::string::const_iterator begin;
	std::string::const_iterator end;
};

class Template;

class Context
{
public:
	// null
	Context();

	// top level
	Context(Object obj, Template const& tmp);

	// partial
	Context(Context const& ctx, Template const& tmp, Tag tag);

	// section
	Context(Context const& ctx, Tag tag);

	// for_each
	Context(Object obj, Context const& ctx);

	explicit operator bool() const;

	std::string to_string() const;

	Engine::Iter render(Renderer& renderer) const;

	std::string::const_iterator begin() const;
	std::string::const_iterator end() const;

	void check_end(Tag const& end_tag) const;

private:
	Object object;
	Context const* parent = nullptr;
	Template const& template_;
	Tag tag_;
};

} // namespace mustache

#endif /* MUSTACHE_CONTEXT_HPP */
