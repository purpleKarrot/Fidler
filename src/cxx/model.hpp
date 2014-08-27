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

#ifndef FIDLER_CXX_MODEL_HPP
#define FIDLER_CXX_MODEL_HPP

#include "typesystem.hpp"
#include "initializer.hpp"
#include "../util/karma.hpp"

#include <fidler/ast/model.hpp>
namespace ast = fidler::ast;

namespace cxx
{

class ModelGrammar: public util::karma_grammar<ast::Model()>
{
public:
	ModelGrammar();

private:
	util::karma_rule<ast::Model()> model_;

	TypeGrammar type_;
	TypeDefinitionGrammar type_definition_;
	InitializerGrammar initializer_;

	util::karma_rule<fidler::ast::TypeCollection()> type_collection_;
	util::karma_rule<fidler::ast::Interface()> interface_;
	util::karma_rule<fidler::ast::Method()> method_;
	util::karma_rule<fidler::ast::Broadcast()> broadcast_;
	util::karma_rule<fidler::ast::Argument()> argument_;
	util::karma_rule<fidler::ast::ConstantDef()> constant_def_;
};

} // namespace cxx

#endif /* FIDLER_CXX_MODEL_HPP */
