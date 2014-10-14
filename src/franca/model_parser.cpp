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

#include "model_parser.hpp"
#include "model_reflection.hpp"

#include <boost/spirit/home/qi/action.hpp>
#include <boost/spirit/home/qi/auxiliary.hpp>
#include <boost/spirit/home/qi/char.hpp>
#include <boost/spirit/home/qi/directive.hpp>
#include <boost/spirit/home/qi/numeric.hpp>
#include <boost/spirit/home/qi/operator.hpp>
#include <boost/spirit/home/qi/string.hpp>

#if BOOST_VERSION >= 105600
# include <boost/phoenix.hpp>
#else
# include <boost/spirit/home/phoenix.hpp>
#endif

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

ModelParser::ModelParser() :
		ModelParser::base_type(model_)
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
		> annotation_
		> -id_
		> '{'
		> -version_
		> *type_definition_
		> *constant_def_
		> '}'
		;

	interface_
		%= "interface"
		> annotation_
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
		> annotation_
		> type_
		> qi::matches[qi::lit('[') > qi::lit(']')]
		> id_
		> qi::matches["readonly"]
		> qi::matches["noSubscriptions"]
		;

	method_
		%= "method"
		> annotation_
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
		> annotation_
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
		> -('=' > initializer_)
		;

	quoted_string_
		%= qi::lexeme['"' > +(qi::char_ - '"') > '"']
		;

	fqn_
		%= qi::raw[id_ % '.']
		//%= qi::lexeme[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'))]
		;

	qi::on_error<qi::fail>(model_,
			boost::phoenix::bind(error_handler, _1, _2, _3, _4));
}

} // namespace franca
