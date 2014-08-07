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

#ifndef FIDLER_CXX_TYPESYSTEM_HPP
#define FIDLER_CXX_TYPESYSTEM_HPP

#include "expression.hpp"
#include "../util/karma.hpp"

#include <fidler/ast/typesystem.hpp>
namespace ast = fidler::ast;

namespace cxx
{

class TypeGrammar:
		public util::karma_grammar<ast::Type()>
{
public:
	TypeGrammar();

private:
	util::karma_rule<ast::Type()> type_;

	util::karma_rule<ast::IntegerInterval()> integer_interval_;

	util::karma_symbols<ast::BuiltinType> builtin_type_;
};

class TypeDefinitionGrammar:
		public util::karma_grammar<ast::TypeDefinition()>
{
public:
	TypeDefinitionGrammar();

private:
	util::karma_rule<ast::TypeDefinition()> type_definition_;

	util::karma_rule<ast::ArrayType()> array_type_;
	util::karma_rule<ast::EnumerationType()> enumeration_type_;
	util::karma_rule<ast::StructType()> struct_type_;
	util::karma_rule<ast::UnionType()> union_type_;
	util::karma_rule<ast::MapType()> map_type_;
	util::karma_rule<ast::AliasType()> alias_type_;

	util::karma_rule<ast::Enumerator()> enumerator_;
	util::karma_rule<ast::Field()> field_;

	TypeGrammar type_;
	ExpressionGrammar expression_;
};

} // namespace cxx

#endif /* FIDLER_CXX_TYPESYSTEM_HPP */
