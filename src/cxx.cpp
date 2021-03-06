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

#include <fidler/cxx.hpp>
#include <fstream>

#include <boost/spirit/home/karma/generate.hpp>

#include "cxx/model.hpp"

namespace fidler
{

bool write_cxx(const char* filename, ast::Model const& model)
{
	std::ofstream file(filename);
	std::ostream_iterator<char> sink(file);
	cxx::ModelGrammar grammar;
	return boost::spirit::karma::generate(sink, grammar, model);
}

void print_cxx_initializer(std::ostream& out,
		ast::InitializerExpression const& init, ast::Type const& /*type*/)
{
	cxx::InitializerGrammar grammar;
	std::ostream_iterator<char> sink(out);
	if (!boost::spirit::karma::generate(sink, grammar, init))
	{
		out.setstate(std::ios::failbit);
	}
}

} // namespace fidler
