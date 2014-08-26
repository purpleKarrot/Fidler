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

#ifndef FRANCA_CONTRACT_GENERATOR_HPP
#define FRANCA_CONTRACT_GENERATOR_HPP

#include <fidler/ast/contract.hpp>
#include "../util/karma.hpp"

#include "initializer_generator.hpp"
#include "expression_generator.hpp"
#include "typesystem_generator.hpp"

namespace franca
{

class ContractGenerator:
		public util::karma_grammar<fidler::ast::Contract()>
{
public:
	ContractGenerator();

private:
	util::karma_rule<fidler::ast::Contract()> contract_;

	util::karma_rule<fidler::ast::StateGraph()> state_graph_;
	util::karma_rule<fidler::ast::State()> state_;
	util::karma_rule<fidler::ast::Transition()> transition_;

	util::karma_rule<fidler::ast::Block()> block_;
	util::karma_rule<fidler::ast::Statement()> statement_;
	util::karma_rule<fidler::ast::Assignment()> assignment_;
	util::karma_rule<fidler::ast::IfClause()> if_clause_;

	util::karma_rule<fidler::ast::Declaration()> declaration_;

	TypeGenerator type_;
	ExpressionGenerator expression_;
	InitializerGenerator initializer_;

	util::karma_symbols<fidler::ast::Trigger> trigger_;
	util::karma_symbols<fidler::ast::Assignment::Operator> assignment_op_;
};

} // namespace franca

#endif /* FRANCA_CONTRACT_GENERATOR_HPP */
