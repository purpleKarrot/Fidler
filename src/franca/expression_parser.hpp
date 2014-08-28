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

#ifndef FRANCA_EXPRESSION_PARSER_HPP
#define FRANCA_EXPRESSION_PARSER_HPP

#include "comment_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
namespace qi = boost::spirit::qi;

#include <fidler/ast/expression.hpp>
namespace ast = fidler::ast;

namespace franca
{

class ExpressionParser:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, ast::Expression()>
{
public:
	ExpressionParser();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	template<typename T>
	using Symbols = boost::spirit::qi::symbols<char, T>;

	Rule<ast::Expression()> expression_;
	Rule<std::pair<ast::Expression, ast::Expression>()> expression_pair_;

	Rule<ast::Identifier()> identifier_;
	Rule<ast::CurrentError()> current_error_;

	Rule<ast::PrimaryExpression()> primary_expr_;
	Rule<ast::PrefixExpression()> prefix_expr_;
	Rule<ast::PostfixExpression()> postfix_expr_;

	Rule<ast::PostfixExpression::MemberAccess()> postfix_member_access_;
	Rule<ast::PostfixExpression::Subscript()> postfix_subscript_;

	Rule<ast::MultiplicativeExpression()> multiplicative_expr_;
	Rule<ast::AdditiveExpression()> additive_expr_;
	Rule<ast::ShiftExpression()> shift_expr_;
	Rule<ast::RelationalExpression()> relational_expr_;
	Rule<ast::EqualityExpression()> equality_expr_;
	Rule<ast::AndExpression()> and_expr_;
	Rule<ast::ExclusiveOrExpression()> exclusive_or_expr_;
	Rule<ast::InclusiveOrExpression()> inclusive_or_expr_;
	Rule<ast::LogicalAndExpression()> logical_and_expr_;
	Rule<ast::LogicalOrExpression()> logical_or_expr_;
	Rule<ast::NullCoalescingExpression()> null_coalescing_expr_;

	Symbols<bool> bool_;
	Symbols<ast::PrefixExpression::Op> prefix_op_;
	Symbols<ast::EqualityExpression::op> equality_op_;
	Symbols<ast::RelationalExpression::op> relational_op_;
	Symbols<ast::ShiftExpression::op> shift_op_;
	Symbols<ast::AdditiveExpression::op> additive_op_;
	Symbols<ast::MultiplicativeExpression::op> multiplicative_op_;

	Rule<std::string()> id_;
};

} // namespace franca

#endif /* FRANCA_EXPRESSION_PARSER_HPP */
