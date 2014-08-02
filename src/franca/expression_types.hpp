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

#ifndef FRANCA_EXPRESSION_TYPES_HPP
#define FRANCA_EXPRESSION_TYPES_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace franca
{

struct DoubleConstant
{
	double val;
};

struct FloatConstant
{
	float val;
};

struct IntegerConstant
{
	int val;
};

struct BooleanConstant
{
	bool val;
};

struct StringConstant
{
	std::string val;
};

struct CurrentError
{
};

struct QualifiedElementRef
{
	std::string path;
};

struct Expression;
struct UnaryExpression;

using PrimaryExpression = boost::variant
<
	QualifiedElementRef,
	CurrentError,
	FloatConstant,
	DoubleConstant,
	IntegerConstant,
	BooleanConstant,
	StringConstant,
	boost::recursive_wrapper<Expression>,
	boost::recursive_wrapper<UnaryExpression>
>;

struct UnaryExpression
{
	enum Op
	{
		plus, minus, negate, complement
	};

	Op op;
	PrimaryExpression expr;
};

struct MultiplicativeExpression
{
	enum op
	{
		mul, div, mod
	};

	using Right = std::pair<op, PrimaryExpression>;

	PrimaryExpression left;
	std::vector<Right> right;
};

struct AdditiveExpression
{
	enum op
	{
		plus, minus
	};

	using Right = std::pair<op, MultiplicativeExpression>;

	MultiplicativeExpression left;
	std::vector<Right> right;
};

struct ShiftExpression
{
	enum op
	{
		lshift, rshift
	};

	using Right = std::pair<op, AdditiveExpression>;

	AdditiveExpression left;
	std::vector<Right> right;
};

struct RelationalExpression
{
	enum op
	{
		lt, le, gt, ge
	};

	using Right = std::pair<op, ShiftExpression>;

	ShiftExpression left;
	boost::optional<Right> right;
};

struct EqualityExpression
{
	enum op
	{
		eq, ne
	};

	using Right = std::pair<op, RelationalExpression>;

	RelationalExpression left;
	boost::optional<Right> right; // std::vector ?
};

struct AndExpression
{
	using Self = boost::recursive_wrapper<AndExpression>;

	EqualityExpression left;
	boost::optional<Self> right;
};

struct ExclusiveOrExpression
{
	using Self = boost::recursive_wrapper<ExclusiveOrExpression>;

	AndExpression left;
	boost::optional<Self> right;
};

struct InclusiveOrExpression
{
	using Self = boost::recursive_wrapper<InclusiveOrExpression>;

	ExclusiveOrExpression left;
	boost::optional<Self> right;
};

struct LogicalAndExpression
{
	using Self = boost::recursive_wrapper<LogicalAndExpression>;

	InclusiveOrExpression left;
	boost::optional<Self> right;
};

struct LogicalOrExpression
{
	using Self = boost::recursive_wrapper<LogicalOrExpression>;

	LogicalAndExpression left;
	boost::optional<Self> right;
};

struct Expression
{
	using TernaryPart = std::pair
	<
		boost::recursive_wrapper<Expression>,
		boost::recursive_wrapper<Expression>
	>;

	LogicalOrExpression left;
	boost::optional<TernaryPart> right;
};

} // namespace franca

#endif /* FRANCA_EXPRESSION_TYPES_HPP */
