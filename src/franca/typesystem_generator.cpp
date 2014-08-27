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

#include "typesystem_generator.hpp"
#include "typesystem_reflection.hpp"

#include <boost/spirit/home/karma.hpp>

namespace franca
{

TypeGenerator::TypeGenerator() :
		TypeGenerator::base_type(type_)
{
	using fidler::ast::BuiltinType;
	namespace karma = boost::spirit::karma;

	type_
		%= builtin_type_ | integer_interval_ | karma::string
		;

	integer_interval_
		%= "Integer("
		<< karma::int_
		<< ", "
		<< karma::int_
		<< ")"
		;

	builtin_type_.add
		(BuiltinType::undefined,  "undefined" )
		(BuiltinType::Int8,       "Int8"      )
		(BuiltinType::UInt8,      "UInt8"     )
		(BuiltinType::Int16,      "Int16"     )
		(BuiltinType::UInt16,     "UInt16"    )
		(BuiltinType::Int32,      "Int32"     )
		(BuiltinType::UInt32,     "UInt32"    )
		(BuiltinType::Int64,      "Int64"     )
		(BuiltinType::UInt64,     "UInt64"    )
		(BuiltinType::Boolean,    "Boolean"   )
		(BuiltinType::String,     "String"    )
		(BuiltinType::Float,      "Float"     )
		(BuiltinType::Double,     "Double"    )
		(BuiltinType::ByteBuffer, "ByteBuffer")
		;
}

TypeDefinitionGenerator::TypeDefinitionGenerator() :
		TypeDefinitionGenerator::base_type(type_definition_)
{
	namespace karma = boost::spirit::karma;

	type_definition_
		%= array_type_
		| enumeration_type_
		| struct_type_
		| union_type_
		| map_type_
		| alias_type_
		;

	array_type_
		%= "  array "
		<< karma::string
		<< " of "
		<< type_
		<< '\n'
		;

	alias_type_
		%= "  typedef "
		<< karma::string
		<< " is "
		<< type_
		<< '\n'
		;

	enumeration_type_
		%= "  enumeration "
		<< karma::string
		<< -(" extends " << karma::string)
		<< "\n  {\n"
		<< *enumerator_
		<< "  }\n"
		;

	struct_type_
		%= "  struct "
		<< karma::string
		<< -(" extends " << karma::string)
		<< (!karma::bool_(true) | " polymorphic")
		<< "\n  {\n"
		<< *field_
		<< "  }\n"
		;

	union_type_
		%= "  union "
		<< karma::string
		<< -(" extends " << karma::string)
		<< "\n  {\n"
		<< *field_
		<< "  }\n"
		;

	map_type_
		%= "  map "
		<< karma::string
		<< " { "
		<< type_
		<< " to "
		<< type_
		<< " }\n"
		;

	field_
		%= "    "
		<< type_
		<< (!karma::bool_(true) | "[]")
		<< ' '
		<< karma::string
		<< '\n'
		;

	enumerator_
		%= "    "
		<< karma::string
		<< -(" = " << expression_)
		<< '\n'
		;
}

} // namespace franca
