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

#include "model.hpp"

#include <boost/spirit/home/karma.hpp>

#include <fidler/util/case_convert.hpp>
#include <fidler/util/reflect.hpp>

FIDLER_REFLECT(ast::Model,
	(name)
	(type_collections)
	(interfaces)
)

FIDLER_REFLECT(ast::TypeCollection,
	(name)
	(types)
	(constants)
	(name)
)

FIDLER_REFLECT(fidler::ast::Interface,
	(name)
	(types)
	(methods)
	(broadcasts)
	(constants)
)

FIDLER_REFLECT(ast::Method,
	(name)
	(in_args)
)

FIDLER_REFLECT(ast::Broadcast,
	(name)
	(out_args)
)

FIDLER_REFLECT(ast::Argument,
	(is_array)
	(type)
	(is_array)
	(name)
)

FIDLER_REFLECT(ast::ConstantDef,
	(type)
	(is_array)
	(name)
	(rhs)
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
		<< *interface_
		;

	type_collection_
		%= -("namespace " << karma::string[fidler::snake_case] << "\n{\n")
		<< *type_definition_
		<< *constant_def_
		<< -("} // namespace " << karma::string << "\n\n")
		;

	interface_
		%= "class "
		<< karma::string[fidler::pascal_case]
		<< "\n{\n"
		<< *type_definition_
		<< *method_
		<< *broadcast_
		<< *constant_def_
		<< "};\n"
		;

	method_
		%= "  void "
		<< karma::string[fidler::snake_case]
		<< '('
		<< -(argument_ % ", ")
		<< ");\n\n"
		;

	broadcast_
		%= "  connection on_"
		<< karma::string[fidler::snake_case]
		<< "(std::function<void("
		<< -(argument_ % ", ")
		<< ")> handler);\n\n"
		;

	argument_
		%= (!karma::bool_(true) | "std::vector<")
		<< type_
		<< (!karma::bool_(true) | '>')
		<< " const& "
		<< karma::string
		;

	constant_def_
		%= "const "
		<< type_
		<< (!karma::bool_(true) | "[]")
		<< ' '
		<< karma::string
		<< " = "
		<< initializer_
		<< ";\n"
		;
}

} // namespace cxx
