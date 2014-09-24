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

#include "context.hpp"

namespace mustache
{

// top level
Context::Context(Object obj, Template const& tmp) :
		object(std::move(obj)), template_(tmp)
{
}

// partial
Context::Context(Context const& ctx, Template const& tmp, Tag tag) :
		parent(&ctx), template_(tmp), tag_(std::move(tag))
{
}

// section
Context::Context(Context const& ctx, Tag tag) :
		parent(&ctx), template_(ctx.template_), tag_(std::move(tag))
{
	std::cout << "=========================================================\n";
	std::cout << "Looking up '" << tag_.name << "'.\n";

	for (Context const* c = parent; c; c = c->parent)
	{
		std::cout << "Analyzing '" << c->object.cxx_type() << "'.\n";

		object = c->object.find(tag_.name);
		if (object)
		{
			std::cout << "found!\n";
			return;
		}
	}

	template_.diagnostic(tag_.begin, tag_.end, "Unknown tag!");
}

// for_each
Context::Context(Object obj, Context const& ctx)
	: object(std::move(obj))
	, parent(ctx.parent)
	, template_(ctx.template_)
	, tag_(ctx.tag_)
{
}

Context::operator bool() const
{
	return !object.empty();
}

std::string Context::to_string() const
{
	return object.to_string();
}

Engine::Iter Context::render(Engine const& engine, std::ostream& out) const
{
	Engine::Iter res;
	object.for_each([&](Object obj)
	{
		auto const ctx = Context(std::move(obj), *this);
		res = engine.render(ctx, out);
	});
	return res;
}

std::string::const_iterator Context::begin() const
{
	return parent ? tag_.end : template_.begin();
}

std::string::const_iterator Context::end() const
{
	return template_.end();
}

void Context::check_end(Tag const& end_tag) const
{
	if (!parent)
	{
		template_.diagnostic(end_tag.begin, end_tag.end, "Unexpected end tag!");
		std::cout << '\n';
	}
	else if (tag_.name != end_tag.name)
	{
		template_.diagnostic(end_tag.begin, end_tag.end, "End tag does not match");
		template_.diagnostic(tag_.begin, tag_.end, "start tag from here.");
		std::cout << '\n';
	}
}

std::string Context::tag() const
{
	return parent ? tag_.name : template_.filename;
}

} // namespace mustache
