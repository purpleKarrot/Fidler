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

#ifndef FRANCA_EXPRESSION_GENERATOR_HPP
#define FRANCA_EXPRESSION_GENERATOR_HPP

#include <fidler/ast/expression.hpp>
#include "../util/karma.hpp"

namespace franca
{

class ExpressionGenerator:
		public util::karma_grammar<fidler::ast::Expression()>
{
public:
	ExpressionGenerator();

private:
	util::karma_rule<fidler::ast::Expression()> expression_;
	util::karma_rule<std::pair<fidler::ast::Expression, fidler::ast::Expression>()> expression_pair_;

	util::karma_rule<fidler::ast::Identifier()> identifier_;
	util::karma_rule<fidler::ast::CurrentError()> current_error_;

	util::karma_rule<fidler::ast::PrimaryExpression()> primary_expr_;
	util::karma_rule<fidler::ast::PrefixExpression()> prefix_expr_;
	util::karma_rule<fidler::ast::PostfixExpression()> postfix_expr_;

	util::karma_rule<fidler::ast::PostfixExpression::MemberAccess()> postfix_member_access_;
	util::karma_rule<fidler::ast::PostfixExpression::Subscript()> postfix_subscript_;

	util::karma_rule<fidler::ast::MultiplicativeExpression()> multiplicative_expr_;
	util::karma_rule<fidler::ast::AdditiveExpression()> additive_expr_;
	util::karma_rule<fidler::ast::ShiftExpression()> shift_expr_;
	util::karma_rule<fidler::ast::RelationalExpression()> relational_expr_;
	util::karma_rule<fidler::ast::EqualityExpression()> equality_expr_;
	util::karma_rule<fidler::ast::AndExpression()> and_expr_;
	util::karma_rule<fidler::ast::ExclusiveOrExpression()> exclusive_or_expr_;
	util::karma_rule<fidler::ast::InclusiveOrExpression()> inclusive_or_expr_;
	util::karma_rule<fidler::ast::LogicalAndExpression()> logical_and_expr_;
	util::karma_rule<fidler::ast::LogicalOrExpression()> logical_or_expr_;
	util::karma_rule<fidler::ast::NullCoalescingExpression()> null_coalescing_expr_;

	util::karma_symbols<fidler::ast::PrefixOperator> prefix_op_;
	util::karma_symbols<fidler::ast::EqualityOperator> equality_op_;
	util::karma_symbols<fidler::ast::RelationalOperator> relational_op_;
	util::karma_symbols<fidler::ast::ShiftOperator> shift_op_;
	util::karma_symbols<fidler::ast::AdditiveOperator> additive_op_;
	util::karma_symbols<fidler::ast::MultiplicativeOperator> multiplicative_op_;
};

} // namespace franca

#endif /* FRANCA_EXPRESSION_GENERATOR_HPP */
