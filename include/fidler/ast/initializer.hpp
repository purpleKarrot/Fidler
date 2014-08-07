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

#ifndef FIDLER_AST_INITIALIZER_HPP
#define FIDLER_AST_INITIALIZER_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <fidler/ast/expression.hpp>

namespace fidler
{
namespace ast
{

struct CompoundInitializer;
struct BracketInitializer;

using InitializerExpression = boost::variant
<
	Expression,
	boost::recursive_wrapper<CompoundInitializer>,
	boost::recursive_wrapper<BracketInitializer>
>;

struct FieldInitializer
{
	std::string element;
	InitializerExpression value;
};

// for array type initializers, the second element is omitted
struct ElementInitializer
{
	InitializerExpression first;
	boost::optional<InitializerExpression> second;
};

// this is used for struct and union types
struct CompoundInitializer
{
	std::vector<FieldInitializer> elements;
};

// this is used for array and map types
struct BracketInitializer
{
	std::vector<ElementInitializer> elements;
};

} // namespace ast
} // namespace fidler

#endif /* FIDLER_AST_INITIALIZER_HPP */
