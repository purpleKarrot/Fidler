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

namespace ast
{

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
	enum Op
	{
		plus, minus, negate, complement
	};

	boost::optional<Op> op;
	PostfixExpression expr;
};

struct MultiplicativeExpression
{
	enum op
	{
		mul, div, mod
	};

	using Right = std::pair<op, PrefixExpression>;

	PrefixExpression left;
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

	AndExpression() = default;
	AndExpression(Self const& other) :
		AndExpression(other.get())
	{
	}

	EqualityExpression left;
	boost::optional<Self> right;
};

struct ExclusiveOrExpression
{
	using Self = boost::recursive_wrapper<ExclusiveOrExpression>;

	ExclusiveOrExpression() = default;
	ExclusiveOrExpression(Self const& other) :
		ExclusiveOrExpression(other.get())
	{
	}

	AndExpression left;
	boost::optional<Self> right;
};

struct InclusiveOrExpression
{
	using Self = boost::recursive_wrapper<InclusiveOrExpression>;

	InclusiveOrExpression() = default;
	InclusiveOrExpression(Self const& other) :
		InclusiveOrExpression(other.get())
	{
	}

	ExclusiveOrExpression left;
	boost::optional<Self> right;
};

struct LogicalAndExpression
{
	using Self = boost::recursive_wrapper<LogicalAndExpression>;

	LogicalAndExpression() = default;
	LogicalAndExpression(Self const& other) :
		LogicalAndExpression(other.get())
	{
	}

	InclusiveOrExpression left;
	boost::optional<Self> right;
};

struct LogicalOrExpression
{
	using Self = boost::recursive_wrapper<LogicalOrExpression>;

	LogicalOrExpression() = default;
	LogicalOrExpression(Self const& other) :
		LogicalOrExpression(other.get())
	{
	}

	LogicalAndExpression left;
	boost::optional<Self> right;
};

struct NullCoalescingExpression
{
	using Right = boost::recursive_wrapper<NullCoalescingExpression>;

	NullCoalescingExpression() = default;
	NullCoalescingExpression(Right const& other) :
		NullCoalescingExpression(other.get())
	{
	}

	LogicalOrExpression left;
	boost::optional<Right> right;
};

struct Expression
{
	using Right = std::pair
	<
		boost::recursive_wrapper<Expression>,
		boost::recursive_wrapper<Expression>
	>;

	Expression() = default;
	Expression(boost::recursive_wrapper<Expression> const& other) :
			Expression(other.get())
	{
	}

	NullCoalescingExpression left;
	boost::optional<Right> right;
};

} // namespace ast

#endif /* FIDLER_AST_EXPRESSION_HPP */
