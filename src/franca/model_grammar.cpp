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

#include "model_grammar.hpp"

#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_directive.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_eoi.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(franca::Version,
	(int, major)
	(int, minor)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Attribute,
	(franca::Type, type)
	(bool, is_array)
	(std::string, name)
	(bool, read_only)
	(bool, no_subscriptions)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Argument,
	(franca::Type, type)
	(bool, is_array)
	(std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Model,
	(std::string, name)
	(std::vector<std::string>, imports)
	(std::vector<franca::TypeCollection>, type_collections)
	(std::vector<franca::Interface>, interfaces)
)

BOOST_FUSION_ADAPT_STRUCT(franca::TypeCollection,
	(boost::optional<std::string>, name)
	(boost::optional<franca::Version>, version)
	(std::vector<franca::TypeDefinition>, types)
	(std::vector<franca::ConstantDef>, constants)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Interface,
	(std::string, name)
	(boost::optional<std::string>, base)
	(std::vector<std::string>, managed_interfaces)
	(boost::optional<franca::Version>, version)
	(std::vector<franca::Attribute>, attributes)
	(std::vector<franca::Method>, methods)
	(std::vector<franca::Broadcast>, broadcasts)
	(boost::optional<franca::Contract>, contract)
	(std::vector<franca::TypeDefinition>, types)
	(std::vector<franca::ConstantDef>, constants)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Method,
	(std::string, name)
	(bool, fire_and_forget)
	(std::vector<franca::Argument>, in_args)
	(std::vector<franca::Argument>, out_args)
	(franca::Error, error)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Broadcast,
	(std::string, name)
	(bool, selective)
	(std::vector<franca::Argument>, out_args)
)

BOOST_FUSION_ADAPT_STRUCT(franca::ConstantDef,
	(franca::Type, type)
	(bool, is_array)
	(std::string, name)
	(franca::InitializerExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Enumerator,
	(std::string, name)
	(franca::Expression, value)
)

namespace franca
{

void error_handler(const char* begin, const char* end, const char* where,
		boost::spirit::qi::info const& what)
{
	std::reverse_iterator<const char*> rev_end(begin);
	std::reverse_iterator<const char*> rev_where(where);

	auto const line_begin = std::find(rev_where, rev_end, '\n').base();
	auto const line_end = std::find(where, end, '\n');

	auto const line_nr = 1 + std::count(begin, where, '\n');

	std::cout
		<< "Expecting " << what << " in line " << line_nr << std::endl
		<< std::string(line_begin, where)
		<< "\033[1;31m"
		<< std::string(where, line_end)
		<< "\033[0m"
		<< std::endl
		;
}

ModelGrammar::ModelGrammar() :
		ModelGrammar::base_type(model_)
{
	namespace qi = boost::spirit::qi;
	using namespace boost::spirit::qi::labels;
	using boost::phoenix::at_c;
	using boost::phoenix::push_back;

	model_
		= "package"
		> fqn_                 [at_c<0>(_val) = _1]
		> *import_             [push_back(at_c<1>(_val), _1)]
		> *( type_collection_  [push_back(at_c<2>(_val), _1)]
		   | interface_        [push_back(at_c<3>(_val), _1)]
		   )
		> qi::eoi
		;

	import_
		= "import"
		//> qi::omit["model" | (qi::lexeme[fqn_ > -qi::lit(".*")] > "from")]
		> qi::omit["model" | (fqn_ > -qi::lit(".*") > "from")]
		> quoted_string_
		;

	type_collection_
		%= "typeCollection"
		> -id_
		> '{'
		> -version_
		> *type_definition_
		> *constant_def_
		> '}'
		;

	interface_
		%= "interface"
		> id_
		> -("extends" > fqn_)
		> -("manages" > (id_ % ','))
		> '{'
		> -version_
		> *attribute_
		> *method_
		> *broadcast_
		> -contract_
		> *type_definition_
		> *constant_def_
		> '}'
		;

	version_
		%= qi::lit("version")
		> '{'
		> qi::lit("major") > qi::int_
		> qi::lit("minor") > qi::int_
		> '}'
		;

	attribute_
		%= "attribute"
		> type_
		> qi::matches[qi::lit('[') > qi::lit(']')]
		> id_
		> qi::matches["readonly"]
		> qi::matches["noSubscriptions"]
		;

	method_
		%= "method"
		> id_
		> qi::matches["fireAndForget"]
		> '{'
		> -(qi::lit("in")  > '{' > *argument_ > '}')
		> -(qi::lit("out") > '{' > *argument_ > '}')
		> -("error" > error_)
		> '}'
		;

	error_
		%= fqn_
		| ('{' > *enumerator_ > '}')
		;

	enumerator_
		%= id_
		>> -('=' > expression_)
		;

	broadcast_
		%= "broadcast"
		> id_
		> qi::matches["selective"]
		> '{'
		> -(qi::lit("out") > '{' > *argument_ > '}')
		> '}'
		;

	constant_def_
		%= "const"
		> type_
		> qi::matches[qi::lit('[') > qi::lit(']')]
		> id_
		> '='
		> initializer_
		;

	argument_
		%= type_
		> qi::matches[qi::lit('[') > qi::lit(']')]
		> id_
		;

	quoted_string_
		%= qi::lexeme['"' > +(qi::char_ - '"') > '"']
		;

	fqn_
		%= qi::raw[id_ % '.']
		//%= qi::lexeme[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;

	qi::on_error<qi::fail>(model_,
			boost::phoenix::bind(error_handler, _1, _2, _3, _4));
}

} // namespace franca