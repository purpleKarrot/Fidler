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

#ifndef FIDLER_CXX_EXPRESSION_HPP
#define FIDLER_CXX_EXPRESSION_HPP

#include "../util/karma.hpp"

#include <fidler/ast/expression.hpp>
namespace ast = fidler::ast;

namespace cxx
{

class ExpressionGrammar: public util::karma_grammar<ast::Expression()>
{
public:
	ExpressionGrammar();

private:
	util::karma_rule<ast::Expression()> expression_;
	util::karma_rule<std::pair<ast::Expression, ast::Expression>()> expression_pair_;

	util::karma_rule<ast::Identifier()> identifier_;
	util::karma_rule<ast::CurrentError()> current_error_;

	util::karma_rule<ast::PrimaryExpression()> primary_expr_;
	util::karma_rule<ast::PrefixExpression()> prefix_expr_;
	util::karma_rule<ast::PostfixExpression()> postfix_expr_;

	util::karma_rule<ast::PostfixExpression::MemberAccess()> postfix_member_access_;
	util::karma_rule<ast::PostfixExpression::Subscript()> postfix_subscript_;

	util::karma_rule<ast::MultiplicativeExpression()> multiplicative_expr_;
	util::karma_rule<ast::AdditiveExpression()> additive_expr_;
	util::karma_rule<ast::ShiftExpression()> shift_expr_;
	util::karma_rule<ast::RelationalExpression()> relational_expr_;
	util::karma_rule<ast::EqualityExpression()> equality_expr_;
	util::karma_rule<ast::AndExpression()> and_expr_;
	util::karma_rule<ast::ExclusiveOrExpression()> exclusive_or_expr_;
	util::karma_rule<ast::InclusiveOrExpression()> inclusive_or_expr_;
	util::karma_rule<ast::LogicalAndExpression()> logical_and_expr_;
	util::karma_rule<ast::LogicalOrExpression()> logical_or_expr_;
	util::karma_rule<ast::NullCoalescingExpression()> null_coalescing_expr_;

	util::karma_symbols<ast::PrefixOperator> prefix_op_;
	util::karma_symbols<ast::EqualityOperator> equality_op_;
	util::karma_symbols<ast::RelationalOperator> relational_op_;
	util::karma_symbols<ast::ShiftOperator> shift_op_;
	util::karma_symbols<ast::AdditiveOperator> additive_op_;
	util::karma_symbols<ast::MultiplicativeOperator> multiplicative_op_;

	util::karma_rule<std::string()> id_;
};

} // namespace cxx

#endif /* FIDLER_CXX_EXPRESSION_HPP */
