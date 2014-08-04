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

#include "typesystem_grammar.hpp"
#include "util/reflect.hpp"

#include <boost/spirit/home/qi.hpp>

FIDLER_REFLECT(franca::IntegerInterval,
	(lower_bound)
	(upper_bound)
)

FIDLER_REFLECT(franca::ArrayType,
	(name)
	(element_type)
)

FIDLER_REFLECT(franca::AliasType,
	(name)
	(actual_type)
)

FIDLER_REFLECT(franca::StructType,
	(name)
	(base)
	(is_polymorphic)
	(elements)
)

FIDLER_REFLECT(franca::UnionType,
	(name)
	(base)
	(elements)
)

FIDLER_REFLECT(franca::MapType,
	(name)
	(key_type)
	(value_type)
)

FIDLER_REFLECT(franca::Field,
	(type)
	(is_array)
	(name)
)

FIDLER_REFLECT(franca::Enumerator,
	(name)
	(value)
)

FIDLER_REFLECT(franca::EnumerationType,
	(name)
	(base)
	(enumerators)
)

namespace franca
{

TypeGrammar::TypeGrammar() :
		TypeGrammar::base_type(type_)
{
	namespace qi = boost::spirit::qi;

	type_
		%= integer_interval_
		| builtin_type_
		| fqn_
		;

	integer_interval_
		%= qi::lit("Integer")
		>> ( '('
		   > (qi::int_ | "minInt" >> qi::attr(INT_MIN))
		   > ','
		   > (qi::int_ | "maxInt" >> qi::attr(INT_MAX))
		   > ')'
		   )
		;

	builtin_type_.add
		("undefined",  BuiltinType::undefined)
		("Int8",       BuiltinType::Int8)
		("UInt8",      BuiltinType::UInt8)
		("Int16",      BuiltinType::Int16)
		("UInt16",     BuiltinType::UInt16)
		("Int32",      BuiltinType::Int32)
		("UInt32",     BuiltinType::UInt32)
		("Int64",      BuiltinType::Int64)
		("UInt64",     BuiltinType::UInt64)
		("Boolean",    BuiltinType::Boolean)
		("String",     BuiltinType::String)
		("Float",      BuiltinType::Float)
		("Double",     BuiltinType::Double)
		("ByteBuffer", BuiltinType::ByteBuffer)
		("Integer",    BuiltinType::Int32)
		;

	fqn_
		%= qi::raw[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

TypeDefinitionGrammar::TypeDefinitionGrammar() :
		TypeDefinitionGrammar::base_type(type_definition_)
{
	namespace qi = boost::spirit::qi;

	type_definition_
		%= array_type_
		| enumeration_type_
		| struct_type_
		| union_type_
		| map_type_
		| alias_type_
		;

	array_type_
		%= "array"
		> id_
		> "of"
		> type_
		;

	alias_type_
		%= "typedef"
		> id_
		> "is"
		> type_
		;

	enumeration_type_
		%= "enumeration"
		> id_
		> -("extends" > fqn_)
		> '{'
		> *enumerator_
		> '}'
		;

	struct_type_
		%= "struct"
		> id_
		> -("extends" > fqn_)
		> qi::matches["polymorphic"]
		> '{'
		> *field_
		> '}'
		;

	union_type_
		%= "union"
		> id_
		> -("extends" > fqn_)
		> '{'
		> *field_
		> '}'
		;

	map_type_
		%= "map"
		> id_
		> '{'
		> type_
		> "to"
		> type_
		> '}'
		;

	field_
		%= type_
		> qi::matches[qi::lit('[') > qi::lit(']')]
		> id_
		;

	enumerator_
		%= id_
		>> -('=' > expression_)
		;

	fqn_
		%= qi::raw[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

} // namespace franca
