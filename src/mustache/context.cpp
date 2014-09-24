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

static Template dummy_template;

// null
Context::Context() :
		template_(dummy_template)
{
}

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
	for (Context const* c = parent; c && !object; c = c->parent)
	{
		object = c->object.find(tag_.name);
	}

	// TODO: output diagnostic, fail
}

// for_each
Context::Context(Object obj, Context const& ctx) :
		object(std::move(obj)), parent(ctx.parent), template_(ctx.template_)
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

Engine::Iter Context::render(Renderer& renderer) const
{
	Engine::Iter res;
	object.for_each([&](Object obj)
	{
//		TODO: add index for loops?
		Context ctx(std::move(obj), *this);
		res = renderer.render(ctx);
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

} // namespace mustache
