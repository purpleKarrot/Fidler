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

#ifndef FRANCA_EXPRESSION_GRAMMAR_HPP
#define FRANCA_EXPRESSION_GRAMMAR_HPP

#include "expression_types.hpp"
#include "comment_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
namespace qi = boost::spirit::qi;

namespace franca
{

class ExpressionGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, Expression()>
{
public:
	ExpressionGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	template<typename T>
	using Symbols = boost::spirit::qi::symbols<char, T>;

	Rule<Expression()> expression_;

	Rule<DoubleConstant()> double_constant_;
	Rule< FloatConstant()> float_constant_;
	Rule< IntegerConstant()> integer_constant_;
	Rule< BooleanConstant()> boolean_constant_;
	Rule< StringConstant()> string_constant_;
	Rule< CurrentError()> current_error_;
	Rule< QualifiedElementRef()> qualified_element_;

	Rule<UnaryExpression()> unary_expr_;
	Rule<PrimaryExpression()> primary_expr_;

	Rule<MultiplicativeExpression()> multiplicative_expr_;
	Rule<AdditiveExpression()> additive_expr_;
	Rule<ShiftExpression()> shift_expr_;
	Rule<RelationalExpression()> relational_expr_;
	Rule<EqualityExpression()> equality_expr_;
	Rule<AndExpression()> and_expr_;
	Rule<ExclusiveOrExpression()> exclusive_or_expr_;
	Rule<InclusiveOrExpression()> inclusive_or_expr_;
	Rule<LogicalAndExpression()> logical_and_expr_;
	Rule<LogicalOrExpression()> logical_or_expr_;

	Symbols<bool> bool_;
	Symbols<UnaryExpression::Op> unary_op_;
	Symbols<EqualityExpression::op> equality_op_;
	Symbols<RelationalExpression::op> relational_op_;
	Symbols<ShiftExpression::op> shift_op_;
	Symbols<AdditiveExpression::op> additive_op_;
	Symbols<MultiplicativeExpression::op> multiplicative_op_;

	Rule<std::string()> fqn_;
	Rule<std::string()> id_;
};

} // namespace franca

#endif /* FRANCA_EXPRESSION_GRAMMAR_HPP */
