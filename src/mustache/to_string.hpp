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

#ifndef MUSTACHE_TO_STRING_HPP
#define MUSTACHE_TO_STRING_HPP

#include <string>
#include <cxxabi.h>
#include <iostream>
#include <boost/optional.hpp>

namespace mustache
{

inline std::string to_string(std::string t)
{
	return t;
}

template<typename T>
inline std::string to_string(T const&)
{
	int status = 0;
	std::cout
		<< "STUB: 'std::string to_string("
		<< abi::__cxa_demangle(typeid(T).name(), 0, 0, &status)
		<< " const&)' is not implemented.\n"
		;
	return "";
}

template<typename T>
inline std::string to_string(boost::optional<T> const& value)
{
	return value ? to_string(*value) : "";
}

} // namespace mustache

#endif /* MUSTACHE_TO_STRING_HPP */
