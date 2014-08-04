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

#include <boost/spirit/home/qi.hpp>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(franca::IntegerInterval,
	(int, lower_bound)
	(int, upper_bound)
)

BOOST_FUSION_ADAPT_STRUCT(franca::ArrayType,
	(std::string, name)
	(franca::Type, element_type)
)

BOOST_FUSION_ADAPT_STRUCT(franca::AliasType,
	(std::string, name)
	(franca::Type, actual_type)
)

BOOST_FUSION_ADAPT_STRUCT(franca::StructType,
	(std::string, name)
	(boost::optional<std::string>, base)
	(bool, is_polymorphic)
	(std::vector<franca::Field>, elements)
)

BOOST_FUSION_ADAPT_STRUCT(franca::UnionType,
	(std::string, name)
	(boost::optional<std::string>, base)
	(std::vector<franca::Field>, elements)
)

BOOST_FUSION_ADAPT_STRUCT(franca::MapType,
	(std::string, name)
	(franca::Type, key_type)
	(franca::Type, value_type)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Field,
	(franca::Type, type)
	(bool, is_array)
	(std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Enumerator,
	(std::string, name)
	(franca::Expression, value)
)

BOOST_FUSION_ADAPT_STRUCT(franca::EnumerationType,
	(std::string, name)
	(boost::optional<std::string>, base)
	(std::vector<franca::Enumerator>, enumerators)
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
