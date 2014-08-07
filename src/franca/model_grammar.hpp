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

#ifndef FRANCA_MODEL_GRAMMAR_HPP
#define FRANCA_MODEL_GRAMMAR_HPP

#include "comment_grammar.hpp"
#include "contract_grammar.hpp"
#include "typesystem_grammar.hpp"
#include "initializer_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>

#include <fidler/ast/model.hpp>
namespace ast = fidler::ast;

namespace franca
{

class ModelGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, ast::Model()>
{
public:
	ModelGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	Rule<ast::Model()> model_;

	TypeGrammar type_;
	TypeDefinitionGrammar type_definition_;
	ExpressionGrammar expression_;
	InitializerGrammar initializer_;
	ContractGrammar contract_;

	Rule<ast::Version()> version_;
	Rule<std::string()> import_;
	Rule<ast::Attribute()> attribute_;
	Rule<ast::Argument()> argument_;
	Rule<ast::Error()> error_;
	Rule<ast::Enumerator()> enumerator_;
	Rule<ast::Method()> method_;
	Rule<ast::Broadcast()> broadcast_;
	Rule<ast::ConstantDef()> constant_def_;
	Rule<ast::TypeCollection()> type_collection_;
	Rule<ast::Interface()> interface_;

	Rule<std::string()> quoted_string_;
	Rule<std::string()> fqn_;
	Rule<std::string()> id_;
};

} // namespace franca

#endif /* FRANCA_MODEL_GRAMMAR_HPP */
