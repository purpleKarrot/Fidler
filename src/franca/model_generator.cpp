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

#include "model_generator.hpp"
#include "model_reflection.hpp"

#include <boost/spirit/home/karma.hpp>

namespace franca
{

ModelGenerator::ModelGenerator() :
		ModelGenerator::base_type(model_)
{
	namespace karma = boost::spirit::karma;

	model_
		%= "package "
		<< karma::string
		<< "\n"
		<< *import_
		<< *type_collection_
		<< *interface_
		;

	import_
		%= "import model from \""
		<< karma::string
		<< "\"\n"
		;

	type_collection_
		%= "typeCollection"
		<< -(' ' << karma::string)
		<< "\n{\n"
		<< -("  " << version_)
		<< *type_definition_
		<< *constant_def_
		<< "}\n"
		;

	interface_
		%= "interface "
		<< karma::string
		<< -(" extends " << karma::string)
		<< -karma::buffer[" manages " << (karma::string % ", ")]
		<< "\n{\n"
		<< -version_
		<< *attribute_
		<< *method_
		<< *broadcast_
		<< -contract_
		<< *type_definition_
		<< *constant_def_
		<< "}\n"
		;

	version_
		%= "  version { major "
		<< karma::int_
		<< " minor "
		<< karma::int_
		<< " }\n"
		;

	attribute_
		%= "  attribute "
		<< type_
		<< (!karma::bool_(true) | "[]")
		<< ' '
		<< karma::string
		<< (!karma::bool_(true) | " readonly")
		<< (!karma::bool_(true) | " noSubscriptions")
		<< karma::eol
		;

	method_
		%= "  method "
		<< karma::string
		<< (!karma::bool_(true) | " fireAndForget")
		<< "\n  {\n"
		<< -karma::buffer["    in" << argument_list_]
		<< -karma::buffer["    out" << argument_list_]
		<< -karma::buffer["    error" << error_ << "\n"]
		<< "  }\n"
		;

	error_
		%= (' ' << karma::string)
		| ("\n    {\n" << *("      " << enumerator_ << '\n') << "    }")
		;

	enumerator_
		%= karma::string
		<< -(" = " << expression_)
		;

	broadcast_
		%= "  broadcast "
		<< karma::string
		<< (!karma::bool_(true) | " selective")
		<< "\n  {\n"
		<< -karma::buffer["    out" << argument_list_]
		<< "  }\n"
		;

	constant_def_
		%= "  const "
		<< type_
		<< (!karma::bool_(true) | "[]")
		<< karma::string
		<< " = "
		<< initializer_
		<< karma::eol
		;

	argument_list_
		%= "\n    {\n"
		<< +("      " << argument_ << '\n')
		<< "    }\n"
		;

	argument_
		%= type_
		<< (!karma::bool_(true) | "[]")
		<< ' '
		<< karma::string
		;
}

} // namespace franca
