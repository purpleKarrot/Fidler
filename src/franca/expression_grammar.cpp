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

#include "expression_grammar.hpp"

#include <boost/spirit/home/qi.hpp>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(franca::Expression,
	(franca::NullCoalescingExpression, left)
	(boost::optional<franca::Expression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::NullCoalescingExpression,
	(franca::LogicalOrExpression, left)
	(boost::optional<franca::NullCoalescingExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::LogicalOrExpression,
	(franca::LogicalAndExpression, left)
	(boost::optional<franca::LogicalOrExpression::Self>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::LogicalAndExpression,
	(franca::InclusiveOrExpression, left)
	(boost::optional<franca::LogicalAndExpression::Self>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::InclusiveOrExpression,
	(franca::ExclusiveOrExpression, left)
	(boost::optional<franca::InclusiveOrExpression::Self>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::ExclusiveOrExpression,
	(franca::AndExpression, left)
	(boost::optional<franca::ExclusiveOrExpression::Self>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::AndExpression,
	(franca::EqualityExpression, left)
	(boost::optional<franca::AndExpression::Self>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::EqualityExpression,
	(franca::RelationalExpression, left)
	(boost::optional<franca::EqualityExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::RelationalExpression,
	(franca::ShiftExpression, left)
	(boost::optional<franca::RelationalExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::ShiftExpression,
	(franca::AdditiveExpression, left)
	(std::vector<franca::ShiftExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::AdditiveExpression,
	(franca::MultiplicativeExpression, left)
	(std::vector<franca::AdditiveExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::MultiplicativeExpression,
	(franca::PrefixExpression, left)
	(std::vector<franca::MultiplicativeExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::PrefixExpression,
	(boost::optional<franca::PrefixExpression::Op>, op)
	(franca::PostfixExpression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(franca::PostfixExpression,
	(franca::PrimaryExpression, base)
	(std::vector<franca::PostfixExpression::Postfix>, postfix)
)

BOOST_FUSION_ADAPT_STRUCT(franca::PostfixExpression::MemberAccess,
	(std::string, member)
)

BOOST_FUSION_ADAPT_STRUCT(franca::PostfixExpression::Subscript,
	(boost::recursive_wrapper<franca::Expression>, value)
)

BOOST_FUSION_ADAPT_STRUCT(franca::Identifier,
	(std::string, name)
)

namespace franca
{

ExpressionGrammar::ExpressionGrammar() :
		ExpressionGrammar::base_type(expression_)
{
	expression_
		%= null_coalescing_expr_
		>> -('?' > expression_ > ':' > expression_)
		;

	null_coalescing_expr_
		%= logical_or_expr_
		>> -("??" >> null_coalescing_expr_)
		;

	logical_or_expr_
		%= logical_and_expr_
		>> -("||" >> logical_or_expr_)
		;

	logical_and_expr_
		%= inclusive_or_expr_
		>> -("&&" >> logical_and_expr_)
		;

	inclusive_or_expr_
		%= exclusive_or_expr_
		>> -('|' >> inclusive_or_expr_)
		;

	exclusive_or_expr_
		%= and_expr_
		>> -('^' >> exclusive_or_expr_)
		;

	and_expr_
		%= equality_expr_
		>> -('&' >> and_expr_)
		;

	equality_expr_
		%= relational_expr_
		>> -(equality_op_ >> relational_expr_)
		;

	relational_expr_
		%= shift_expr_
		>> -(relational_op_ >> shift_expr_)
		;

	shift_expr_
		%= additive_expr_
		>> *(shift_op_ >> additive_expr_)
		;

	additive_expr_
		%= multiplicative_expr_
		>> *(additive_op_ >> multiplicative_expr_)
		;

	multiplicative_expr_
		%= prefix_expr_
		>> *(multiplicative_op_ >> prefix_expr_)
		;

	prefix_expr_
		%= -prefix_op_
		>> postfix_expr_
		;

	postfix_expr_
		%= primary_expr_
		>> *(postfix_member_access_ | postfix_subscript_)
		;

	postfix_member_access_
		%= ('.' > id_)
		;

	postfix_subscript_
		%= ('[' > expression_ > ']')
		;

	primary_expr_
		%= ('(' > expression_ > ')')
		| (qi::double_ >> 'd')
		| (qi::float_ >> 'f')
		| qi::int_
		| bool_
		| qi::lexeme['"' > +(qi::char_ - '"') > '"']
		| current_error_
		| identifier_
		;

	identifier_
		%= id_
		;

	current_error_
		%= qi::lit("errorval")
		|  qi::lit("errordef") //TODO
		;

	bool_.add
		("true", true)
		("false", false)
		;

	prefix_op_.add
		("+", PrefixExpression::plus)
		("-", PrefixExpression::minus)
		("!", PrefixExpression::negate)
		("~", PrefixExpression::complement)
		;

	equality_op_.add
		("==", EqualityExpression::eq)
		("!=", EqualityExpression::ne)
		;

	relational_op_.add
		("<", RelationalExpression::lt)
		("<=", RelationalExpression::le)
		(">", RelationalExpression::gt)
		(">=", RelationalExpression::ge)
		;

	shift_op_.add
		("<<", ShiftExpression::lshift)
		(">>", ShiftExpression::rshift)
		;

	additive_op_.add
		("+", AdditiveExpression::plus)
		("-", AdditiveExpression::minus)
		;

	multiplicative_op_.add
		("*", MultiplicativeExpression::mul)
		("/", MultiplicativeExpression::div)
		("%", MultiplicativeExpression::mod)
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

} // namespace franca