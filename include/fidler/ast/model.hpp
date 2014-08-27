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

#ifndef FIDLER_AST_MODEL_HPP
#define FIDLER_AST_MODEL_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <fidler/ast/contract.hpp>
#include <fidler/ast/typesystem.hpp>
#include <fidler/ast/initializer.hpp>

namespace fidler
{
namespace ast
{

struct Version
{
	int major;
	int minor;
};

struct Attribute
{
	boost::optional<AnnotationBlock> comment;
	Type type;
	bool is_array;
	std::string name;
	bool read_only;
	bool no_subscriptions;
};

struct Argument
{
	boost::optional<AnnotationBlock> comment;
	Type type;
	bool is_array;
	std::string name;
};

using Error = boost::variant
<
	std::string,
	std::vector<Enumerator>
>;

struct Method
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	bool fire_and_forget;
	std::vector<Argument> in_args;
	std::vector<Argument> out_args;
	boost::optional<Error> error;
};

struct Broadcast
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	bool selective;
	std::vector<Argument> out_args;
};

struct ConstantDef
{
	Type type;
	bool is_array;
	std::string name;
	InitializerExpression rhs;
};

struct TypeCollection
{
	boost::optional<AnnotationBlock> comment;
	boost::optional<std::string> name;
	boost::optional<Version> version;
	std::vector<TypeDefinition> types;
	std::vector<ConstantDef> constants;
};

struct Interface
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	boost::optional<std::string> base;
	std::vector<std::string> managed_interfaces;
	boost::optional<Version> version;
	std::vector<Attribute> attributes;
	std::vector<Method> methods;
	std::vector<Broadcast> broadcasts;
	boost::optional<Contract> contract;
	std::vector<TypeDefinition> types;
	std::vector<ConstantDef> constants;
};

struct Model
{
	std::string name;
	std::vector<std::string> imports;
	std::vector<TypeCollection> type_collections;
	std::vector<Interface> interfaces;
};

} // namespace ast
} // namespace fidler

#endif /* FIDLER_AST_MODEL_HPP */
