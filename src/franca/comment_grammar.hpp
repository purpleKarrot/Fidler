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

#ifndef FRANCA_COMMENT_GRAMMAR_HPP
#define FRANCA_COMMENT_GRAMMAR_HPP

#include "ast/comment.hpp"

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi/nonterminal/rule.hpp>

namespace franca
{

class SkipGrammar:
		public boost::spirit::qi::grammar<const char*>
{
public:
	SkipGrammar();

private:
	boost::spirit::qi::rule<const char*> skip_;
	boost::spirit::qi::rule<const char*, std::string()> annotation_;
};

class AnnotationGrammar:
		public boost::spirit::qi::grammar<const char*, std::string()>
{
public:
	AnnotationGrammar();

private:
	boost::spirit::qi::rule<const char*, std::string()> annotation_;
};

} // namespace franca

#endif /* FRANCA_COMMENT_GRAMMAR_HPP */
