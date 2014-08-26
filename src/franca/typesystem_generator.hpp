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

#ifndef FRANCA_TYPESYSTEM_GENERATOR_HPP
#define FRANCA_TYPESYSTEM_GENERATOR_HPP

#include <fidler/ast/typesystem.hpp>
#include "../util/karma.hpp"

#include "expression_generator.hpp"

namespace franca
{

class TypeGenerator:
		public util::karma_grammar<fidler::ast::Type()>
{
public:
	TypeGenerator();

private:
	util::karma_rule<fidler::ast::Type()> type_;
	util::karma_rule<fidler::ast::IntegerInterval()> integer_interval_;
	util::karma_symbols<fidler::ast::BuiltinType> builtin_type_;
};

class TypeDefinitionGenerator:
		public util::karma_grammar<fidler::ast::TypeDefinition()>
{
public:
	TypeDefinitionGenerator();

private:
	util::karma_rule<fidler::ast::TypeDefinition()> type_definition_;

	TypeGenerator type_;
	ExpressionGenerator expression_;

	util::karma_rule<fidler::ast::ArrayType()> array_type_;
	util::karma_rule<fidler::ast::EnumerationType()> enumeration_type_;
	util::karma_rule<fidler::ast::StructType()> struct_type_;
	util::karma_rule<fidler::ast::UnionType()> union_type_;
	util::karma_rule<fidler::ast::MapType()> map_type_;
	util::karma_rule<fidler::ast::AliasType()> alias_type_;

	util::karma_rule<fidler::ast::Enumerator()> enumerator_;
	util::karma_rule<fidler::ast::Field()> field_;
};

} // namespace franca

#endif /* FRANCA_TYPESYSTEM_GENERATOR_HPP */
