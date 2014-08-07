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

#include <fidler/util/algorithm.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>
#include <boost/xpressive/regex_primitives.hpp>
#include <boost/xpressive/regex_actions.hpp>

namespace fidler
{

namespace util
{

std::string capitalize(std::string str)
{
	if (!str.empty())
		str[0] = toupper(str[0]);

	return str;
}

std::string str_toupper(std::string str)
{
	transform(begin(str), end(str), begin(str), toupper);
	return str;
}

std::string str_tolower(std::string str)
{
	transform(begin(str), end(str), begin(str), tolower);
	return str;
}

std::string underscore_to_camel(std::string str)
{
	using namespace boost::xpressive;
	sregex static const match = '_' >> (s1 = _);
	return regex_replace(str, match, boost::xpressive::ref(str_toupper)(s1));
}

std::string camel_to_underscore(std::string str)
{
	using namespace boost::xpressive;

	sregex static const match1 = (s1 = _) >> (s2 = upper >> +lower);
	sregex static const match2 = (s1 = lower | digit) >> (s2 = upper);
	auto static const replacement = s1 + "_" + s2;

	str = regex_replace(str, match1, replacement);
	str = regex_replace(str, match2, replacement);

	return str;
}

} // namespace util
} // namespace fidler
