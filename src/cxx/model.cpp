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

#include "cxx/model.hpp"
#include "util/reflect.hpp"

#include <boost/spirit/home/karma.hpp>

FIDLER_REFLECT(ast::Model,
	(name)
	(type_collections)
)

FIDLER_REFLECT(ast::TypeCollection,
	(name)
	(types)
	(constants)
)

FIDLER_REFLECT(ast::ConstantDef,
	(type)
	(is_array)
	(name)
)

namespace cxx
{

ModelGrammar::ModelGrammar() :
		ModelGrammar::base_type(model_)
{
	namespace karma = boost::spirit::karma;

	model_
		%= "package: '"
		<< karma::string
		<< "'\n"
		<< *type_collection_
		;

	type_collection_
		%= "collection '"
		<< -karma::string
		<< "'\n"
		<< *type_definition_
		<< *constant_def_
		;

	constant_def_
		%= "const "
		<< type_
		<< (!karma::bool_(true) | "[]")
		<< karma::string
		<< ";\n"
		;
}

} // namespace cxx
