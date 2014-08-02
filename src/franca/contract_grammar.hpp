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

#ifndef FRANCA_CONTRACT_GRAMMAR_HPP
#define FRANCA_CONTRACT_GRAMMAR_HPP

#include "contract_types.hpp"
#include "comment_grammar.hpp"
#include "initializer_grammar.hpp"
#include "expression_grammar.hpp"
#include "typesystem_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>

namespace franca
{

class ContractGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, Contract()>
{
public:
	ContractGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	Rule<Contract()> contract_;

	Rule<StateGraph()> state_graph_;
	Rule<State()> state_;
	Rule<Transition()> transition_;
	Rule<Guard()> guard_;

	Rule<Block()> block_;
	Rule<Statement()> statement_;
	Rule<Assignment()> assignment_;
	Rule<IfClause()> if_clause_;

	Rule<Declaration()> declaration_;

	TypeGrammar type_;
	ExpressionGrammar expression_;
	InitializerGrammar initializer_;
	Rule<std::string()> fqn_;
	Rule<std::string()> id_;

	boost::spirit::qi::symbols<char, Trigger> trigger_;
};

} // namespace franca

#endif /* FRANCA_CONTRACT_GRAMMAR_HPP */
