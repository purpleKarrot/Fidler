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

#ifndef DBUSXML_TYPE_FACTORY_HPP
#define DBUSXML_TYPE_FACTORY_HPP

#include <fidler/ast/typesystem.hpp>
#include <unordered_map>

namespace dbusxml
{

class TypeFactory
{
	using Type = fidler::ast::Type;
	using TypeDefinition = fidler::ast::TypeDefinition;
	using StringIterator = std::string::const_iterator;

public:
	TypeFactory(std::vector<TypeDefinition>& types);

	Type lookup(std::string const& signature);

private:
	std::string make_name();
	Type parse(StringIterator& it, StringIterator last);

private:
	std::size_t counter = 0;
	std::vector<TypeDefinition>& types;
	std::unordered_map<std::string, std::string> names;
};

} // namespace dbusxml

#endif /* DBUSXML_TYPE_FACTORY_HPP */
