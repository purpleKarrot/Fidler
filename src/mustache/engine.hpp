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

#ifndef MUSTACHE_ENGINE_HPP
#define MUSTACHE_ENGINE_HPP

#include <regex>
#include <unordered_map>
#include <iosfwd>
#include <string>
#include "template.hpp"

namespace mustache
{

class Context;

class Engine
{
public:
	explicit Engine(std::string path);

	Template const* load_template(std::string const& name) const;

	using Iter = Template::Iterator;
	Iter render(Context const& ctx, std::ostream& out) const;

private:
	std::string path;
	std::regex tag_regex;
	mutable std::unordered_map<std::string, Template> templates;
};

} // namespace mustache

#endif /* MUSTACHE_ENGINE_HPP */
