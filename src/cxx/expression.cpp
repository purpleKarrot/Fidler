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

#include "expression.hpp"

#include <boost/spirit/home/karma.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#include <fidler/util/reflect.hpp>

FIDLER_REFLECT(ast::Expression, (left)(right))
FIDLER_REFLECT(ast::NullCoalescingExpression, (left)(right))
FIDLER_REFLECT(ast::LogicalOrExpression, (left)(right))
FIDLER_REFLECT(ast::LogicalAndExpression, (left)(right))
FIDLER_REFLECT(ast::InclusiveOrExpression, (left)(right))
FIDLER_REFLECT(ast::ExclusiveOrExpression, (left)(right))
FIDLER_REFLECT(ast::AndExpression, (left)(right))
FIDLER_REFLECT(ast::EqualityExpression, (left)(right))
FIDLER_REFLECT(ast::RelationalExpression, (left)(right))
FIDLER_REFLECT(ast::ShiftExpression, (left)(right))
FIDLER_REFLECT(ast::AdditiveExpression, (left)(right))
FIDLER_REFLECT(ast::MultiplicativeExpression, (left)(right))
FIDLER_REFLECT(ast::PrefixExpression, (op)(expr))
FIDLER_REFLECT(ast::PostfixExpression, (base)(postfix))
FIDLER_REFLECT(ast::PostfixExpression::MemberAccess, (member))
FIDLER_REFLECT(ast::PostfixExpression::Subscript, (value))
FIDLER_REFLECT(ast::Identifier, (name))

namespace cxx
{

ExpressionGrammar::ExpressionGrammar() :
		ExpressionGrammar::base_type(expression_)
{
	namespace karma = boost::spirit::karma;

	expression_
		%= null_coalescing_expr_
		<< -(" ? " << expression_ << " : " << expression_)
		;

	null_coalescing_expr_
		%= logical_or_expr_
		<< -(" ?? " << null_coalescing_expr_)
		;

	logical_or_expr_
		%= logical_and_expr_
		<< -(" || " << logical_or_expr_)
		;

	logical_and_expr_
		%= inclusive_or_expr_
		<< -(" && " << logical_and_expr_)
		;

	inclusive_or_expr_
		%= exclusive_or_expr_
		<< -(" | " << inclusive_or_expr_)
		;

	exclusive_or_expr_
		%= and_expr_
		<< -(" ^ " << exclusive_or_expr_)
		;

	and_expr_
		%= equality_expr_
		<< -(" & " << and_expr_)
		;

	equality_expr_
		%= relational_expr_
		<< -(equality_op_ << relational_expr_)
		;

	relational_expr_
		%= shift_expr_
		<< -(relational_op_ << shift_expr_)
		;

	shift_expr_
		%= additive_expr_
		<< *(shift_op_ << additive_expr_)
		;

	additive_expr_
		%= multiplicative_expr_
		<< *(additive_op_ << multiplicative_expr_)
		;

	multiplicative_expr_
		%= prefix_expr_
		<< *(multiplicative_op_ << prefix_expr_)
		;

	prefix_expr_
		%= -prefix_op_
		<< postfix_expr_
		;

	postfix_expr_
		%= primary_expr_
		<< *(postfix_member_access_ | postfix_subscript_)
		;

	postfix_member_access_
		%= ('.' << karma::string)
		;

	postfix_subscript_
		%= ('[' << expression_ << ']')
		;

	primary_expr_
		%= ('(' << expression_ << ')')
		| karma::double_
		| (karma::float_ << 'f')
		| karma::int_
		| karma::bool_
		| ('"' << karma::string << '"')
		| current_error_
		| identifier_
		;

	identifier_
		%= karma::eps
		<< karma::string
		;

	current_error_
		%= karma::eps
		<< karma::lit("errorval")
		;

	prefix_op_.add
		(ast::PrefixExpression::plus, "+")
		(ast::PrefixExpression::minus, "-")
		(ast::PrefixExpression::negate, "!")
		(ast::PrefixExpression::complement, "~")
		;

	equality_op_.add
		(ast::EqualityExpression::eq, " == ")
		(ast::EqualityExpression::ne, " != ")
		;

	relational_op_.add
		(ast::RelationalExpression::lt, " < ")
		(ast::RelationalExpression::le, " <= ")
		(ast::RelationalExpression::gt, " > ")
		(ast::RelationalExpression::ge, " >= ")
		;

	shift_op_.add
		(ast::ShiftExpression::lshift, " << ")
		(ast::ShiftExpression::rshift, " >> ")
		;

	additive_op_.add
		(ast::AdditiveExpression::plus, " + ")
		(ast::AdditiveExpression::minus, " - ")
		;

	multiplicative_op_.add
		(ast::MultiplicativeExpression::mul, " * ")
		(ast::MultiplicativeExpression::div, " / ")
		(ast::MultiplicativeExpression::mod, " % ")
		;
}

} // namespace cxx
