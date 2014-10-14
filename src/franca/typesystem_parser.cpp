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

#include "typesystem_parser.hpp"
#include "typesystem_reflection.hpp"

#include <boost/spirit/home/qi/auxiliary.hpp>
#include <boost/spirit/home/qi/char.hpp>
#include <boost/spirit/home/qi/directive.hpp>
#include <boost/spirit/home/qi/numeric.hpp>
#include <boost/spirit/home/qi/operator.hpp>
#include <boost/spirit/home/qi/string.hpp>

namespace franca
{

TypeParser::TypeParser() :
		TypeParser::base_type(type_)
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
		   > (qi::long_long | "minInt" >> qi::attr(LLONG_MIN))
		   > ','
		   > (qi::long_long | "maxInt" >> qi::attr(LLONG_MAX))
		   > ')'
		   )
		;

	builtin_type_.add
		("undefined",  ast::BuiltinType::undefined)
		("Int8",       ast::BuiltinType::Int8)
		("UInt8",      ast::BuiltinType::UInt8)
		("Int16",      ast::BuiltinType::Int16)
		("UInt16",     ast::BuiltinType::UInt16)
		("Int32",      ast::BuiltinType::Int32)
		("UInt32",     ast::BuiltinType::UInt32)
		("Int64",      ast::BuiltinType::Int64)
		("UInt64",     ast::BuiltinType::UInt64)
		("Boolean",    ast::BuiltinType::Boolean)
		("String",     ast::BuiltinType::String)
		("Float",      ast::BuiltinType::Float)
		("Double",     ast::BuiltinType::Double)
		("ByteBuffer", ast::BuiltinType::ByteBuffer)
		("Integer",    ast::BuiltinType::Int32)
		;

	fqn_
		%= qi::raw[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

TypeDefinitionParser::TypeDefinitionParser() :
		TypeDefinitionParser::base_type(type_definition_)
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
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'))]
		;
}

} // namespace franca
