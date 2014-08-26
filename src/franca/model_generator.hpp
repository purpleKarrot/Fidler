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

#ifndef FRANCA_MODEL_GENERATOR_HPP
#define FRANCA_MODEL_GENERATOR_HPP

#include <fidler/ast/model.hpp>
#include "../util/karma.hpp"

#include "expression_generator.hpp"
#include "initializer_generator.hpp"
#include "typesystem_generator.hpp"

namespace franca
{

class ModelGenerator: public util::karma_grammar<fidler::ast::Model()>
{
public:
	ModelGenerator();

private:
	util::karma_rule<fidler::ast::Model()> model_;

	TypeGenerator type_;
	TypeDefinitionGenerator type_definition_;
	InitializerGenerator initializer_;
	util::karma_rule<fidler::ast::Contract()> contract_;
	ExpressionGenerator expression_;

	util::karma_rule<std::string()> import_;
	util::karma_rule<fidler::ast::Version()> version_;
	util::karma_rule<fidler::ast::Attribute()> attribute_;
	util::karma_rule<fidler::ast::Argument()> argument_;
	util::karma_rule<fidler::ast::Error()> error_;
	util::karma_rule<fidler::ast::Enumerator()> enumerator_;
	util::karma_rule<fidler::ast::Method()> method_;
	util::karma_rule<fidler::ast::Broadcast()> broadcast_;
	util::karma_rule<fidler::ast::ConstantDef()> constant_def_;
	util::karma_rule<fidler::ast::TypeCollection()> type_collection_;
	util::karma_rule<fidler::ast::Interface()> interface_;
};

} // namespace franca

#endif /* FRANCA_MODEL_GENERATOR_HPP */
