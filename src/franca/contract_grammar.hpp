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

#include "comment_grammar.hpp"
#include "initializer_parser.hpp"
#include "expression_parser.hpp"
#include "typesystem_parser.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>

#include <fidler/ast/contract.hpp>
namespace ast = fidler::ast;

namespace franca
{

class ContractGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, ast::Contract()>
{
public:
	ContractGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	template<typename T>
	using Symbols = boost::spirit::qi::symbols<char, T>;

	Rule<ast::Contract()> contract_;

	Rule<ast::StateGraph()> state_graph_;
	Rule<ast::State()> state_;
	Rule<ast::Transition()> transition_;

	Rule<ast::Block()> block_;
	Rule<ast::Statement()> statement_;
	Rule<ast::Assignment()> assignment_;
	Rule<ast::IfClause()> if_clause_;

	Rule<ast::Declaration()> declaration_;

	TypeParser type_;
	ExpressionParser expression_;
	InitializerParser initializer_;
	Rule<std::string()> fqn_;
	Rule<std::string()> id_;

	Symbols<ast::Trigger> trigger_;
	Symbols<ast::Assignment::Operator> assignment_op_;
};

} // namespace franca

#endif /* FRANCA_CONTRACT_GRAMMAR_HPP */
