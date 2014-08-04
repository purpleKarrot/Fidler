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

#ifndef FIDLER_UTIL_KARMA_HPP
#define FIDLER_UTIL_KARMA_HPP

#include <iosfwd>
#include <boost/spirit/include/karma_symbols.hpp>
#include <boost/spirit/include/karma_nonterminal.hpp>

namespace util
{

using output_iterator = std::ostream_iterator<char>;

template<
    typename T1 = boost::spirit::unused_type,
    typename T2 = boost::spirit::unused_type,
    typename T3 = boost::spirit::unused_type>
using karma_grammar = boost::spirit::karma::grammar<output_iterator, T1, T2, T3>;

template<
    typename T1 = boost::spirit::unused_type,
    typename T2 = boost::spirit::unused_type,
    typename T3 = boost::spirit::unused_type>
using karma_rule = boost::spirit::karma::rule<output_iterator, T1, T2, T3>;

template<typename T>
using karma_symbols = boost::spirit::karma::symbols<T, char const*>;

} // namespace util

#endif /* FIDLER_UTIL_KARMA_HPP */
