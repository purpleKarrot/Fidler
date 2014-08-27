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
#include <boost/detail/lightweight_test.hpp>

static std::string to_camel_case(std::string str)
{
	fidler::camel_case(str);
	return str;
}

static std::string to_pascal_case(std::string str)
{
	fidler::pascal_case(str);
	return str;
}

static std::string to_snake_case(std::string str)
{
	fidler::snake_case(str);
	return str;
}

int main()
{
	BOOST_TEST_EQ(to_camel_case("helloWorld"),  "helloWorld");
	BOOST_TEST_EQ(to_camel_case("HelloWorld"),  "helloWorld");
	BOOST_TEST_EQ(to_camel_case("hello_world"), "helloWorld");

	BOOST_TEST_EQ(to_pascal_case("helloWorld"),  "HelloWorld");
	BOOST_TEST_EQ(to_pascal_case("HelloWorld"),  "HelloWorld");
	BOOST_TEST_EQ(to_pascal_case("hello_world"), "HelloWorld");

	BOOST_TEST_EQ(to_snake_case("HELLOWorld"),  "hello_world");
	BOOST_TEST_EQ(to_snake_case("helloWorld"),  "hello_world");
	BOOST_TEST_EQ(to_snake_case("HelloWorld"),  "hello_world");
	BOOST_TEST_EQ(to_snake_case("hello_world"), "hello_world");

	return boost::report_errors();
}
