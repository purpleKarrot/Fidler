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

#include <fidler/dbusxml.hpp>
#include "type_factory.hpp"
#include "pugixml.hpp"
#include <iostream>

namespace fidler
{

ast::Method const read_method(pugi::xml_node const& node,
		dbusxml::TypeFactory& type_factory)
{
	ast::Method method;
	method.name = node.attribute("name").as_string("");

	for (auto&& arg : node.children("arg"))
	{
		ast::Argument argument;
		argument.name = arg.attribute("name").as_string("");
		argument.type = type_factory.lookup(
				arg.attribute("type").as_string(""));
		if (arg.attribute("direction").as_string("") == "in")
		{
			method.in_args.push_back(argument);
		}
		else if (arg.attribute("direction").as_string("") == "out")
		{
			method.out_args.push_back(argument);
		}
	}

	return method;
}

ast::Broadcast const read_signal(pugi::xml_node const& node,
		dbusxml::TypeFactory& type_factory)
{
	ast::Broadcast broadcast;
	return broadcast;
}

ast::Attribute const read_property(pugi::xml_node const& node,
		dbusxml::TypeFactory& type_factory)
{
	ast::Attribute attribute;
	attribute.name = node.attribute("name").as_string("");
	return attribute;
}

ast::Interface const read_interface(pugi::xml_node const& node)
{
	ast::Interface interface;
	dbusxml::TypeFactory type_factory(interface.types);
	interface.name = node.attribute("name").as_string("");

	for (auto&& method : node.children("method"))
	{
		interface.methods.push_back(read_method(method, type_factory));
	}

	for (auto&& signal : node.children("signal"))
	{
		interface.broadcasts.push_back(read_signal(signal, type_factory));
	}

	for (auto&& property : node.children("property"))
	{
		interface.attributes.push_back(read_property(property, type_factory));
	}

	return interface;
}

bool read_dbusxml(const char* filename, ast::Model& model)
{
	pugi::xml_document document;
	pugi::xml_parse_result result = document.load_file(filename);
	if (!result)
	{
		std::cout << result.description() << std::endl;
		return false;
	}

	for (auto&& node : document.children("node"))
	{
		for (auto&& interface : node.children("interface"))
		{
			model.interfaces.push_back(read_interface(interface));
		}
	}
}

} // namespace fidler
