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
	(franca::LogicalOrExpression, left)
	(boost::optional<franca::Expression::TernaryPart>, right)
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
	(franca::PrimaryExpression, left)
	(std::vector<franca::MultiplicativeExpression::Right>, right)
)

BOOST_FUSION_ADAPT_STRUCT(franca::UnaryExpression,
	(franca::UnaryExpression::Op, op)
	(franca::PrimaryExpression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(franca::BooleanConstant,
	(bool, val)
)

BOOST_FUSION_ADAPT_STRUCT(franca::IntegerConstant,
	(int, val)
)

BOOST_FUSION_ADAPT_STRUCT(franca::FloatConstant,
	(float, val)
)

BOOST_FUSION_ADAPT_STRUCT(franca::DoubleConstant,
	(double, val)
)

BOOST_FUSION_ADAPT_STRUCT(franca::StringConstant,
	(std::string, val)
)

BOOST_FUSION_ADAPT_STRUCT(franca::QualifiedElementRef,
	(std::string, path)
)

namespace franca
{

ExpressionGrammar::ExpressionGrammar() :
		ExpressionGrammar::base_type(expression_)
{
	expression_
		%= logical_or_expr_
		>> -('?' > expression_ > ':' > expression_)
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
		%= primary_expr_
		>> *(multiplicative_op_ >> primary_expr_)
		;

	primary_expr_
		%= '(' > expression_ > ')'
		| unary_expr_
		| double_constant_
		| float_constant_
		| integer_constant_
		| boolean_constant_
		| string_constant_
		| current_error_
		| qualified_element_
		;

	unary_expr_
		%= unary_op_
		>> primary_expr_
		;

	boolean_constant_
		%= bool_
		;

	integer_constant_
		%= qi::int_
		;

	float_constant_
		%= qi::float_ >> 'f'
		;

	double_constant_
		%= qi::double_ >> 'd'
		;

	string_constant_
		%= qi::lexeme['"' > +(qi::char_ - '"') > '"'];
		;

	qualified_element_
		%= fqn_
		;

	bool_.add
		("true", true)
		("false", false)
		;

	unary_op_.add
		("+", UnaryExpression::plus)
		("-", UnaryExpression::minus)
		("!", UnaryExpression::negate)
		("~", UnaryExpression::complement)
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

	fqn_
		%= qi::raw[id_ % '.']
		//%= qi::lexeme[id_ % '.']
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | '_') >> *(qi::alnum | '_')]
		;
}

} // namespace franca
