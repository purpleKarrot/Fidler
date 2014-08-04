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

#include "contract_grammar.hpp"
#include "util/reflect.hpp"

#include <boost/spirit/home/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

FIDLER_REFLECT(franca::Contract,
	(variables)
	(state_graph)
)

FIDLER_REFLECT(franca::Declaration,
	(type)
	(is_array)
	(name)
	(rhs)
)

FIDLER_REFLECT(franca::StateGraph,
	(initial)
	(states)
)

FIDLER_REFLECT(franca::State,
	(name)
	(transitions)
)

FIDLER_REFLECT(franca::Transition,
	(trigger)
	(event)
	(guard)
	(to)
	(action)
)

FIDLER_REFLECT(franca::Block,
	(statements)
)

FIDLER_REFLECT(franca::Assignment,
	(lhs)
	(op)
	(rhs)
)

FIDLER_REFLECT(franca::IfClause,
	(condition)
	(then_statements)
)

namespace franca
{

ContractGrammar::ContractGrammar() :
		ContractGrammar::base_type(contract_)
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
		("call",    Trigger::call)
		("respond", Trigger::respond)
		("signal",  Trigger::signal)
		("set",     Trigger::set)
		("update",  Trigger::update)
		("error",   Trigger::error)
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
		("=", Assignment::assign)
		("*=", Assignment::mul_assign)
		("/=", Assignment::div_assign)
		("%=", Assignment::mod_assign)
		("+=", Assignment::plus_assign)
		("-=", Assignment::minus_assign)
		("<<=", Assignment::lshift_assign)
		(">>=", Assignment::rshift_assign)
		("&=", Assignment::and_assign)
		("^=", Assignment::xor_assign)
		("|=", Assignment::or_assign)
		;

	fqn_
		%= qi::raw[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

} // namespace franca
