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

#include <boost/spirit/home/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(franca::Contract,
	(std::vector<franca::Declaration>, variables)
	(franca::StateGraph, state_graph)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Declaration,
	(franca::Type, type)
	(bool, is_array)
	(std::string, name)
	(boost::optional<franca::InitializerExpression>, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(franca::StateGraph,
	(std::string, initial)
	(std::vector<franca::State>, states)
)

BOOST_FUSION_ADAPT_STRUCT(franca::State,
	(std::string, name)
	(std::vector<franca::Transition>, transitions)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Transition,
	(franca::Trigger, trigger)
	(std::string, event)
	(boost::optional<franca::Guard>, guard)
	(std::string, to)
	(boost::optional<franca::Block>, action)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Guard,
	(franca::Expression, condition)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Block,
	(std::vector<franca::Statement>, statements)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Assignment,
	(std::string, lhs)
	(franca::Expression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(franca::IfClause,
	(franca::Expression, condition)
	(std::vector<franca::Statement>, then_statements)
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
		> -guard_
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

	guard_
		%= '[' > expression_ > ']'
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
		> '='
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

	fqn_
		%= qi::raw[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

} // namespace franca
