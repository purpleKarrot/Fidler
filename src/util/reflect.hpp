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

#ifndef UTIL_REFLECT_HPP
#define UTIL_REFLECT_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define FIDLER_REFLECT_MEMBER(IGNORE, STRUCT, MEMBER)                          \
	(decltype(STRUCT::MEMBER), MEMBER)                                         \

#define FIDLER_REFLECT(STRUCT, MEMBERS)                                        \
	BOOST_FUSION_ADAPT_STRUCT(STRUCT,                                          \
		BOOST_PP_SEQ_FOR_EACH(FIDLER_REFLECT_MEMBER, STRUCT, MEMBERS))         \

#endif /* UTIL_REFLECT_HPP */
