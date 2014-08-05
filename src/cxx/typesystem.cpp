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

#include "cxx/typesystem.hpp"
#include "util/reflect.hpp"

#include <boost/spirit/home/karma.hpp>

FIDLER_REFLECT(ast::IntegerInterval,
	(lower_bound)
	(upper_bound)
)

FIDLER_REFLECT(ast::ArrayType,
	(name)
	(element_type)
)

FIDLER_REFLECT(ast::AliasType,
	(name)
	(actual_type)
)

FIDLER_REFLECT(ast::StructType,
	(name)
	(elements)
)

FIDLER_REFLECT(ast::UnionType,
	(name)
	(elements)
)

FIDLER_REFLECT(ast::MapType,
	(name)
	(key_type)
	(value_type)
)

FIDLER_REFLECT(ast::Field,
	(is_array)
	(type)
	(is_array)
	(name)
)

FIDLER_REFLECT(ast::Enumerator,
	(name)
	(value)
)

FIDLER_REFLECT(ast::EnumerationType,
	(name)
	(enumerators)
)

namespace cxx
{

TypeGrammar::TypeGrammar() :
		TypeGrammar::base_type(type_)
{
	namespace karma = boost::spirit::karma;

	type_
		%= integer_interval_
		| builtin_type_
		| karma::string
		;

	integer_interval_
		%= karma::lit("std::int32_t")
		<< karma::omit[karma::int_]
		<< karma::omit[karma::int_]
		;

	builtin_type_.add
		(ast::BuiltinType::Int8,       "std::int8_t")
		(ast::BuiltinType::UInt8,      "std::uint8_t")
		(ast::BuiltinType::Int16,      "std::int16_t")
		(ast::BuiltinType::UInt16,     "std::uint16_t")
		(ast::BuiltinType::Int32,      "std::int32_t")
		(ast::BuiltinType::UInt32,     "std::uint32_t")
		(ast::BuiltinType::Int64,      "std::int64_t")
		(ast::BuiltinType::UInt64,     "std::uint64_t")
		(ast::BuiltinType::Boolean,    "bool")
		(ast::BuiltinType::String,     "std::string")
		(ast::BuiltinType::Float,      "float")
		(ast::BuiltinType::Double,     "double")
		(ast::BuiltinType::ByteBuffer, "std::vector<std::uint8_t>")
		;
}

TypeDefinitionGrammar::TypeDefinitionGrammar() :
		TypeDefinitionGrammar::base_type(type_definition_)
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
		%= "using "
		<< karma::string
		<< " = std::vector<"
		<< type_
		<< ">;\n"
		;

	alias_type_
		%= "using "
		<< karma::string
		<< " = "
		<< type_
		<< ";\n"
		;

	enumeration_type_
		%= "enum class "
		<< karma::string
		<< "\n{\n"
		<< *("  " << enumerator_)
		<< "};\n"
		;

	struct_type_
		%= "struct "
		<< karma::string
		<< "\n{\n"
		<< *("  " << field_)
		<< "};\n"
		;

	union_type_
		%= "union "
		<< karma::string
		<< "\n{\n"
		<< *("  " << field_)
		<< "};\n"
		;

	map_type_
		%= "using "
		<< karma::string
		<< " = std::map<"
		<< type_
		<< ", "
		<< type_
		<< ">;\n"
		;

	field_
		%= (!karma::bool_(true) | "std::vector<")
		<< type_
		<< (!karma::bool_(true) | '>')
		<< ' '
		<< karma::string
		<< ";\n"
		;

	enumerator_
		%= karma::string
		<< -(" = " << expression_)
		<< ",\n"
		;
}

} // namespace cxx
