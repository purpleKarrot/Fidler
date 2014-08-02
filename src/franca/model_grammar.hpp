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

#include "model_types.hpp"
#include "comment_grammar.hpp"
#include "contract_grammar.hpp"
#include "typesystem_grammar.hpp"
#include "initializer_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>

namespace franca
{

class ModelGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, Model()>
{
public:
	ModelGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	Rule<Model()> model_;

	TypeGrammar type_;
	TypeDefinitionGrammar type_definition_;
	ExpressionGrammar expression_;
	InitializerGrammar initializer_;
	ContractGrammar contract_;

	Rule<Version()> version_;
	Rule<std::string()> import_;
	Rule<Attribute()> attribute_;
	Rule<Argument()> argument_;
	Rule<Error()> error_;
	Rule<Enumerator()> enumerator_;
	Rule<Method()> method_;
	Rule<Broadcast()> broadcast_;
	Rule<ConstantDef()> constant_def_;
	Rule<TypeCollection()> type_collection_;
	Rule<Interface()> interface_;

	Rule<std::string()> quoted_string_;
	Rule<std::string()> fqn_;
	Rule<std::string()> id_;
};

} // namespace franca

#endif /* FRANCA_MODEL_GRAMMAR_HPP */
