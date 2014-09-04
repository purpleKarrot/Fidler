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

#include <fidler/util/case_convert.hpp>
#include <algorithm>

namespace fidler
{

template<typename Iterator>
Iterator make_camel_case(Iterator first, Iterator last)
{
	Iterator result = first;
	while (first != last)
	{
		if (*first == '_' && first + 1 != last)
		{
			*result = std::toupper(*++first);
		}
		else
		{
			*result = *first;
		}

		++result;
		++first;
	}

	return result;
}

void camel_case(std::string& str)
{
	if (!str.empty())
	{
		str.erase(make_camel_case(begin(str), end(str)), end(str));
		str[0] = std::tolower(str[0]);
	}
}

void pascal_case(std::string& str)
{
	if (!str.empty())
	{
		str.erase(make_camel_case(begin(str), end(str)), end(str));
		str[0] = std::toupper(str[0]);
	}
}

void snake_case(std::string& str)
{
	for (auto i = begin(str); i != end(str); ++i)
	{
		if (i + 1 != end(str) && std::isupper(*(i + 1))
				&& (std::islower(*i) || std::isdigit(*i)
						|| (i + 2 != end(str) && std::islower(*(i + 2)))))
		{
			i = str.insert(i + 1, '_');
		}
	}

	lower_case(str);
}

void lower_case(std::string& str)
{
	transform(begin(str), end(str), begin(str), tolower);
}

void upper_case(std::string& str)
{
	transform(begin(str), end(str), begin(str), toupper);
}

} // namespace fidler
