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

#include "initializer_parser.hpp"
#include "initializer_reflection.hpp"

#include <boost/spirit/home/qi.hpp>

namespace franca
{

InitializerParser::InitializerParser() :
		InitializerParser::base_type(initializer_)
{
	initializer_
		%= expression_
		| compound_initializer_
		| bracket_initializer_
		;

	compound_initializer_
		%= '{'
		> -(field_initializer_ % ',')
		> '}'
		;

	bracket_initializer_
		%= '['
		> -(element_initializer_ % ',')
		> ']'
		;

	field_initializer_
		%= id_
		> ':'
		> initializer_
		;

	element_initializer_
		%= initializer_
		> -("=>" > initializer_)
		;

	id_
		%= qi::lexeme[-qi::lit('^') >> (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'))]
		;
}

} // namespace franca
