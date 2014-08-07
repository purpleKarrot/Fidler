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

#ifndef FRANCA_TYPESYSTEM_GRAMMAR_HPP
#define FRANCA_TYPESYSTEM_GRAMMAR_HPP

#include "comment_grammar.hpp"
#include "expression_grammar.hpp"

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_symbols.hpp>

#include <fidler/ast/typesystem.hpp>
namespace ast = fidler::ast;

namespace franca
{

class TypeGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, ast::Type()>
{
public:
	TypeGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	Rule<ast::Type()> type_;

	Rule<std::string()> type_ref_;
	Rule<ast::IntegerInterval()> integer_interval_;

	boost::spirit::qi::symbols<char, ast::BuiltinType> builtin_type_;

	Rule<std::string()> fqn_;
	Rule<std::string()> id_;
};

class TypeDefinitionGrammar:
		public boost::spirit::qi::grammar<const char*, SkipGrammar, ast::TypeDefinition()>
{
public:
	TypeDefinitionGrammar();

private:
	template<typename T>
	using Rule = boost::spirit::qi::rule<const char*, SkipGrammar, T>;

	Rule<ast::TypeDefinition()> type_definition_;

	TypeGrammar type_;
	ExpressionGrammar expression_;

	Rule<ast::ArrayType()> array_type_;
	Rule<ast::EnumerationType()> enumeration_type_;
	Rule<ast::StructType()> struct_type_;
	Rule<ast::UnionType()> union_type_;
	Rule<ast::MapType()> map_type_;
	Rule<ast::AliasType()> alias_type_;

	Rule<ast::Enumerator()> enumerator_;
	Rule<ast::Field()> field_;

	Rule<std::string()> fqn_;
	Rule<std::string()> id_;
};

} // namespace franca

#endif /* FRANCA_TYPESYSTEM_GRAMMAR_HPP */
