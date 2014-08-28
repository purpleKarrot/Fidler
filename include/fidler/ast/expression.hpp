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
	BinaryExpression() = default;
	BinaryExpression(BinaryExpression&&) = default;
	BinaryExpression(BinaryExpression const&) = default;
	BinaryExpression& operator=(BinaryExpression&&) = default;
	BinaryExpression& operator=(BinaryExpression const&) = default;

	template<typename T>
	BinaryExpression(T val) : left{std::move(val)} {}

	template<typename T>
	BinaryExpression& operator=(T val)
	{
		left = std::move(val);
		right = boost::none;
		return *this;
	}

	Left left;
	boost::optional<boost::recursive_wrapper<Right>> right;
};

//TODO: find a better name and/or refactor.
template<typename Operator, typename Operand>
struct OperatorExpression
{
	OperatorExpression() = default;
	OperatorExpression(OperatorExpression&&) = default;
	OperatorExpression(OperatorExpression const&) = default;
	OperatorExpression& operator=(OperatorExpression&&) = default;
	OperatorExpression& operator=(OperatorExpression const&) = default;

	template<typename T>
	OperatorExpression(T val) : left{std::move(val)} {}

	template<typename T>
	OperatorExpression& operator=(T val)
	{
		left = std::move(val);
		right.clear();
		return *this;
	}

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
	PrefixExpression() = default;
	PrefixExpression(PrefixExpression&&) = default;
	PrefixExpression(PrefixExpression const&) = default;
	PrefixExpression& operator=(PrefixExpression&&) = default;
	PrefixExpression& operator=(PrefixExpression const&) = default;

	template<typename T>
	PrefixExpression(T val) : expr{std::move(val)} {}

	template<typename T>
	PrefixExpression& operator=(T val)
	{
		op = boost::none;
		expr = std::move(val);
		return *this;
	}

	boost::optional<PrefixOperator> op;
	PostfixExpression expr;
};

using MultiplicativeExpression =
		OperatorExpression<MultiplicativeOperator, PrefixExpression>;

using AdditiveExpression =
		OperatorExpression<AdditiveOperator, MultiplicativeExpression>;

using ShiftExpression =
		OperatorExpression<ShiftOperator, AdditiveExpression>;

using RelationalExpression =
		OperatorExpression<RelationalOperator, ShiftExpression>;

using EqualityExpression =
		OperatorExpression<EqualityOperator, RelationalExpression>;

struct AndExpression:
		BinaryExpression<EqualityExpression, AndExpression>
{
	using BinaryExpression::BinaryExpression;
};

struct ExclusiveOrExpression:
		BinaryExpression<AndExpression, ExclusiveOrExpression>
{
	using BinaryExpression::BinaryExpression;
};

struct InclusiveOrExpression:
		BinaryExpression<ExclusiveOrExpression, InclusiveOrExpression>
{
	using BinaryExpression::BinaryExpression;
};

struct LogicalAndExpression:
		BinaryExpression<InclusiveOrExpression, LogicalAndExpression>
{
	using BinaryExpression::BinaryExpression;
};

struct LogicalOrExpression:
		BinaryExpression<LogicalAndExpression, LogicalOrExpression>
{
	using BinaryExpression::BinaryExpression;
};

struct NullCoalescingExpression:
		BinaryExpression<LogicalOrExpression, NullCoalescingExpression>
{
	using BinaryExpression::BinaryExpression;
};

struct Expression: BinaryExpression<NullCoalescingExpression,
		std::pair<Expression, Expression>>
{
	using BinaryExpression::BinaryExpression;
};

} // namespace ast
} // namespace fidler

#endif /* FIDLER_AST_EXPRESSION_HPP */
