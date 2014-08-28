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

#ifndef FIDLER_AST_EXPRESSION_HPP
#define FIDLER_AST_EXPRESSION_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace fidler
{
namespace ast
{

enum class PrefixOperator
{
	plus, minus, negate, complement
};

enum class MultiplicativeOperator
{
	mul, div, mod
};

enum class AdditiveOperator
{
	plus, minus
};

enum class ShiftOperator
{
	lshift, rshift
};

enum class RelationalOperator
{
	lt, le, gt, ge
};

enum class EqualityOperator
{
	eq, ne
};

struct Identifier
{
	std::string name;
};

struct CurrentError
{
};

struct Expression;

using PrimaryExpression = boost::variant
<
	Identifier,
	CurrentError,
	float,
	double,
	int,
	bool,
	std::string,
	boost::recursive_wrapper<Expression>
>;

//TODO: find a better name and/or refactor.
template<typename Left, typename Right>
struct BinaryExpression
{
	Left left;
	boost::optional<boost::recursive_wrapper<Right>> right;
};

//TODO: find a better name and/or refactor.
template<typename Operator, typename Operand>
struct OperatorExpression
{
	Operand left;
	std::vector<std::pair<Operator, Operand>> right;
};

struct PostfixExpression
{
	struct MemberAccess
	{
		std::string member;
	};

	struct Subscript
	{
		boost::recursive_wrapper<Expression> value;
	};

	using Postfix = boost::variant
	<
		MemberAccess,
		Subscript
	>;

	PrimaryExpression base;
	std::vector<Postfix> postfix;
};

struct PrefixExpression
{
	boost::optional<PrefixOperator> op;
	PostfixExpression expr;
};

struct MultiplicativeExpression:
		OperatorExpression<MultiplicativeOperator, PrefixExpression>
{
};

struct AdditiveExpression:
		OperatorExpression<AdditiveOperator, MultiplicativeExpression>
{
};

struct ShiftExpression:
		OperatorExpression<ShiftOperator, AdditiveExpression>
{
};

struct RelationalExpression:
		OperatorExpression<RelationalOperator, ShiftExpression>
{
};

struct EqualityExpression:
		OperatorExpression<EqualityOperator, RelationalExpression>
{
};

struct AndExpression:
		BinaryExpression<EqualityExpression, AndExpression>
{
};

struct ExclusiveOrExpression:
		BinaryExpression<AndExpression, ExclusiveOrExpression>
{
};

struct InclusiveOrExpression:
		BinaryExpression<ExclusiveOrExpression, InclusiveOrExpression>
{
};

struct LogicalAndExpression:
		BinaryExpression<InclusiveOrExpression, LogicalAndExpression>
{
};

struct LogicalOrExpression:
		BinaryExpression<LogicalAndExpression, LogicalOrExpression>
{
};

struct NullCoalescingExpression:
		BinaryExpression<LogicalOrExpression, NullCoalescingExpression>
{
};

struct Expression: BinaryExpression<NullCoalescingExpression,
		std::pair<Expression, Expression>>
{
};

} // namespace ast
} // namespace fidler

#endif /* FIDLER_AST_EXPRESSION_HPP */
