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

#include "comment_grammar.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_auxiliary.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>

#include <boost/spirit/home/phoenix.hpp>

namespace franca
{

/**
 * current doxygen documentation string
 */
static std::string cur_dox;

SkipGrammar::SkipGrammar() :
		SkipGrammar::base_type(skip_)
{
	using boost::spirit::_1;
	using boost::spirit::qi::eol;
	using boost::spirit::qi::lit;
	using boost::spirit::qi::char_;
	using boost::spirit::repository::qi::confix;
	namespace ph = boost::phoenix;

	skip_
		= boost::spirit::ascii::space
		| annotation_[ph::ref(cur_dox) = _1]
		| confix("/*", "*/")[*(char_ - "*/")]
		| confix("//", eol)[*(char_ - eol)]
		;

	annotation_ %= confix("<**", "**>")[*(char_ - "**>")];
}

AnnotationGrammar::AnnotationGrammar():
		AnnotationGrammar::base_type(annotation_)
{
	using boost::spirit::eps;
	using boost::spirit::_val;
	namespace ph = boost::phoenix;

	annotation_ = eps[_val = ph::ref(cur_dox), ph::clear(ph::ref(cur_dox))];
}

} // namespace franca
