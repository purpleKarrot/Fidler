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

#include "type_factory.hpp"

namespace dbusxml
{

TypeFactory::TypeFactory(std::vector<TypeDefinition>& types) :
		types(types)
{
}

std::string TypeFactory::make_name()
{
	return "Type" + std::to_string(++counter);
}

TypeFactory::Type TypeFactory::lookup(std::string const& signature)
{
	auto it = std::begin(signature);
	return parse(it, std::end(signature));
}

TypeFactory::Type TypeFactory::parse(StringIterator& it, StringIterator last)
{
	assert(it != last);
	using fidler::ast::BuiltinType;

	switch (*it++)
	{
	case 'a':
		if (*it++ == '{')
		{
			StringIterator s = it;
			fidler::ast::MapType map;

			map.key_type = parse(it, last);
			map.value_type = parse(it, last);

			//assert(*it == '}');

			auto signature = std::string(s, it);
			auto f = names.find(signature);
			if (f != names.end())
			{
				return f->second;
			}

			auto name = make_name();
			names[signature] = name;
			map.name = name;
			types.push_back(map);
			return name;
		}
		else
		{
			StringIterator s = it;
			fidler::ast::ArrayType array;

			array.element_type = parse(it, last);

			auto signature = std::string(s, it);
			auto f = names.find(signature);
			if (f != names.end())
			{
				return f->second;
			}

			auto name = make_name();
			names[signature] = name;
			array.name = name;
			types.push_back(array);
			return name;
		}
		break;
	case '(':
		{
			std::size_t mc = 0;
			StringIterator s = it;
			fidler::ast::StructType type;

			while (it != last && *it != '}')
			{
				fidler::ast::Field field;
				field.name = "m" + std::to_string(++mc);
				field.type = parse(it, last);
				type.elements.push_back(field);
			}

			auto signature = std::string(s, it);
			auto f = names.find(signature);
			if (f != names.end())
			{
				return f->second;
			}

			auto name = make_name();
			names[signature] = name;
			type.name = name;
			types.push_back(type);
			return name;
		}
		break;
	case 'b':
		return BuiltinType::Boolean;
	case 'y':
		return BuiltinType::UInt8;
	case 'n':
		return BuiltinType::Int16;
	case 'q':
		return BuiltinType::UInt16;
	case 'i':
		return BuiltinType::Int32;
	case 'u':
		return BuiltinType::UInt32;
	case 'x':
		return BuiltinType::Int64;
	case 't':
		return BuiltinType::UInt64;
	case 'd':
		return BuiltinType::Double;
	case 's':
		return BuiltinType::String;
	default:
		return BuiltinType::undefined;
	}
}

} /* namespace dbusxml */
