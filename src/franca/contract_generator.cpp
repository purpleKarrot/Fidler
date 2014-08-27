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

#include "contract_generator.hpp"
#include "contract_reflection.hpp"

#include <boost/spirit/home/karma.hpp>

namespace franca
{

ContractGenerator::ContractGenerator() :
		ContractGenerator::base_type(contract_)
{
	namespace karma = boost::spirit::karma;
	using fidler::ast::Assignment;
	using fidler::ast::Trigger;

	contract_
		%= "  contract\n  {\n"
		<< -karma::buffer["    vars\n    {\n" << +declaration_ << "    }\n"]
		<< state_graph_
		<< "  }\n"
		;

	declaration_
		%= "      "
		<< type_
		<< (!karma::bool_(true) | "[]")
		<< ' '
		<< karma::string
		<< -(" = " << initializer_)
		<< ";\n"
		;

	state_graph_
		%= "    PSM\n    {\n      initial "
		<< karma::string
		<< '\n'
		<< *state_
		<< "    }\n"
		;

	state_
		%= "      state "
		<< karma::string
		<< "\n      {\n"
		<< *transition_
		<< "      }\n"
		;

	transition_
		%= "        on "
		<< trigger_
		<< ' '
		<< karma::string
		<< -karma::buffer[" [" << expression_ << ']']
		<< " -> "
		<< karma::string
		<< -block_
		<< karma::eol
		;

	trigger_.add
		(Trigger::call,    "call"   )
		(Trigger::respond, "respond")
		(Trigger::signal,  "signal" )
		(Trigger::set,     "set"    )
		(Trigger::update,  "update" )
		(Trigger::error,   "error"  )
		;

	block_
		%= "\n        {\n"
		<< *statement_
		<< "        }"
		;

	statement_
		%= if_clause_
		| assignment_
		;

	assignment_
		%= "          "
		<< karma::string
		<< assignment_op_
		<< expression_
		<< karma::eol
		;

	if_clause_
		%= "          if ("
		<< expression_
		<< ")\n          {\n"
		<< *statement_
		<< "          }\n"
		;

	assignment_op_.add
		(Assignment::assign,          " = ")
		(Assignment::mul_assign,     " *= ")
		(Assignment::div_assign,     " /= ")
		(Assignment::mod_assign,     " %= ")
		(Assignment::plus_assign,    " += ")
		(Assignment::minus_assign,   " -= ")
		(Assignment::lshift_assign, " <<= ")
		(Assignment::rshift_assign, " >>= ")
		(Assignment::and_assign,     " &= ")
		(Assignment::xor_assign,     " ^= ")
		(Assignment::or_assign,      " |= ")
		;
}

} // namespace franca
