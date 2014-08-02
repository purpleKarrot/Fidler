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

#ifndef FRANCA_TYPESYSTEM_TYPES_HPP
#define FRANCA_TYPESYSTEM_TYPES_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "comment_types.hpp"
#include "expression_types.hpp"

namespace franca
{

enum class BuiltinType
{
	undefined,
	Int8,
	UInt8,
	Int16,
	UInt16,
	Int32,
	UInt32,
	Int64,
	UInt64,
	Boolean,
	String,
	Float,
	Double,
	ByteBuffer,
};

struct IntegerInterval
{
	int lower_bound;
	int upper_bound;
};

using Type = boost::variant
<
	BuiltinType,
	IntegerInterval,
	std::string // resolves to 'TypeDefinition*'
>;

struct ArrayType
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	Type element_type;
};

struct Enumerator
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	Expression value;
};

struct EnumerationType
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	boost::optional<std::string> base;
	std::vector<Enumerator> enumerators;
};

struct Field
{
	boost::optional<AnnotationBlock> comment;
	Type type;
	bool is_array;
	std::string name;
};

struct StructType
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	boost::optional<std::string> base;
	bool is_polymorphic;
	std::vector<Field> elements;
};

struct UnionType
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	boost::optional<std::string> base;
	std::vector<Field> elements;
};

struct MapType
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	Type key_type;
	Type value_type;
};

struct AliasType
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	Type actual_type;
};

using TypeDefinition = boost::variant
<
	ArrayType,
	EnumerationType,
	StructType,
	UnionType,
	MapType,
	AliasType
>;

} // namespace franca

#endif /* FRANCA_TYPESYSTEM_TYPES_HPP */
