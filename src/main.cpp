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

#include <iostream>
#include <fidler/cxx.hpp>
#include <fidler/franca.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace
{

using Read = bool (*)(const char*, fidler::ast::Model&);
using Write = bool (*)(const char*, fidler::ast::Model const&);

struct Format
{
	const char* name;
	const char* ext;
	Read read;
	Write write;
};

Format formats[] =
{
	{ "franca", ".fidl", fidler::read_franca, fidler::write_franca },
	{ "cxx", ".cxx", nullptr, fidler::write_cxx },
};

Read determine_reader(std::string const& filename)
{
	for (auto&& fmt : formats)
	{
		if (fmt.read && fmt.ext && boost::ends_with(filename, fmt.ext))
			return fmt.read;
	}

	return nullptr;
}

Write determine_writer(std::string const& filename)
{
	for (auto&& fmt : formats)
	{
		if (fmt.write && fmt.ext && boost::ends_with(filename, fmt.ext))
			return fmt.write;
	}

	return nullptr;
}

} // namespace

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << "input output\n";
		return -1;
	}

	Read read = determine_reader(argv[1]);
	if (!read)
	{
		std::cerr << "Cannot determine reader.\n";
		return -1;
	}

	Write write = determine_writer(argv[2]);
	if (!write)
	{
		std::cerr << "Cannot determine reader.\n";
		return -1;
	}

	fidler::ast::Model model;

	if (!read(argv[1], model))
	{
		return -1;
	}

	if (!write(argv[2], model))
	{
		return -1;
	}

	return 0;
}
