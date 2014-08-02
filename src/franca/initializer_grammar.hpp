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

#ifndef FRANCA_INITIALIZER_GRAMMAR_HPP
#define FRANCA_INITIALIZER_GRAMMAR_HPP

#include "initializer_types.hpp"
#include "comment_grammar.hpp"
#include "expression_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>

namespace franca
{

class InitializerGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, InitializerExpression()>
{
public:
	InitializerGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	Rule<InitializerExpression()> initializer_;
	Rule<FieldInitializer()> field_initializer_;
	Rule<ElementInitializer()> element_initializer_;
	Rule<CompoundInitializer()> compound_initializer_;
	Rule<BracketInitializer()> bracket_initializer_;

	ExpressionGrammar expression_;
	Rule<std::string()> id_;
};

} // namespace franca

#endif /* FRANCA_INITIALIZER_GRAMMAR_HPP */
