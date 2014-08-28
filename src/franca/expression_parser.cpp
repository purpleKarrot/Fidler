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

#include "expression_parser.hpp"
#include "expression_reflection.hpp"

#include <boost/spirit/home/qi.hpp>

namespace franca
{

ExpressionParser::ExpressionParser() :
		ExpressionParser::base_type(expression_)
{
	using fidler::ast::PrefixOperator;
	using fidler::ast::EqualityOperator;
	using fidler::ast::RelationalOperator;
	using fidler::ast::ShiftOperator;
	using fidler::ast::AdditiveOperator;
	using fidler::ast::MultiplicativeOperator;

	expression_
		%= null_coalescing_expr_
		>> -expression_pair_
		;

	expression_pair_
		%= '?'
		> expression_
		> ':'
		> expression_
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
		>> *(equality_op_ >> relational_expr_)
		;

	relational_expr_
		%= shift_expr_
		>> *(relational_op_ >> shift_expr_)
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
		("+", PrefixOperator::plus)
		("-", PrefixOperator::minus)
		("!", PrefixOperator::negate)
		("~", PrefixOperator::complement)
		;

	equality_op_.add
		("==", EqualityOperator::eq)
		("!=", EqualityOperator::ne)
		;

	relational_op_.add
		("<", RelationalOperator::lt)
		("<=", RelationalOperator::le)
		(">", RelationalOperator::gt)
		(">=", RelationalOperator::ge)
		;

	shift_op_.add
		("<<", ShiftOperator::lshift)
		(">>", ShiftOperator::rshift)
		;

	additive_op_.add
		("+", AdditiveOperator::plus)
		("-", AdditiveOperator::minus)
		;

	multiplicative_op_.add
		("*", MultiplicativeOperator::mul)
		("/", MultiplicativeOperator::div)
		("%", MultiplicativeOperator::mod)
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'))]
		;
}

} // namespace franca
