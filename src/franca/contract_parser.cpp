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

#include "contract_parser.hpp"
#include "contract_reflection.hpp"

#include <boost/spirit/home/qi.hpp>

namespace franca
{

ContractParser::ContractParser() :
		ContractParser::base_type(contract_)
{
	namespace qi = boost::spirit::qi;

	contract_
		%= qi::lit("contract")
		> '{'
		> -(qi::lit("vars") > '{' > *declaration_ > '}' )
		> state_graph_
		> '}'
		;

	declaration_
		%= type_
		> qi::matches[qi::lit('[') > qi::lit(']')]
		> id_
		> -('=' > initializer_)
		> -qi::lit(';')
		;

	state_graph_
		%= qi::lit("PSM")
		> '{'
		> qi::lit("initial")
		> id_
		> *state_
		> '}'
		;

	state_
		%= "state"
		> id_
		> '{'
		> *transition_
		> '}'
		;

	transition_
		%= "on"
		> trigger_
		> id_
		> -('[' > expression_ > ']')
		> "->"
		> id_
		> -block_
		;

	trigger_.add
		("call",    ast::Trigger::call)
		("respond", ast::Trigger::respond)
		("signal",  ast::Trigger::signal)
		("set",     ast::Trigger::set)
		("update",  ast::Trigger::update)
		("error",   ast::Trigger::error)
		;

	block_
		%= '{'
		> *statement_
		> '}'
		;

	statement_
		%= if_clause_
		| assignment_
		;

	assignment_
		%= fqn_
		> assignment_op_
		> expression_
		;

	if_clause_
		%= qi::lit("if")
		> '('
		> expression_
		> ')'
		> '{'
		> *statement_
		> '}'
//		> -("else" > if_clause_)
		;

	assignment_op_.add
		("=", ast::Assignment::assign)
		("*=", ast::Assignment::mul_assign)
		("/=", ast::Assignment::div_assign)
		("%=", ast::Assignment::mod_assign)
		("+=", ast::Assignment::plus_assign)
		("-=", ast::Assignment::minus_assign)
		("<<=", ast::Assignment::lshift_assign)
		(">>=", ast::Assignment::rshift_assign)
		("&=", ast::Assignment::and_assign)
		("^=", ast::Assignment::xor_assign)
		("|=", ast::Assignment::or_assign)
		;

	fqn_
		%= qi::raw[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'))]
		;
}

} // namespace franca
