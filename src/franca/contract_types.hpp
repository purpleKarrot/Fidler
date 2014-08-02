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

#ifndef FRANCA_CONTRACT_TYPES_HPP
#define FRANCA_CONTRACT_TYPES_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "comment_types.hpp"
#include "typesystem_types.hpp"
#include "expression_types.hpp"
#include "initializer_types.hpp"

namespace franca
{

enum class Trigger
{
	call,
	respond,
	signal,
	set,
	update,
	error,
};

struct Guard
{
	Expression condition;
};

struct Assignment
{
	std::string lhs;
	Expression rhs;
};

struct IfClause;

using Statement = boost::variant
<
	Assignment,
	IfClause
>;

struct IfClause
{
	Expression condition;
	std::vector<Statement> then_statements;
	std::vector<Statement> else_statements;
};

using Statement = boost::variant
<
	Assignment,
	IfClause
>;

struct Block
{
	std::vector<Statement> statements;
};

struct Transition
{
	Trigger trigger;
	std::string event;
	boost::optional<Guard> guard;
	std::string to;
	boost::optional<Block> action;
};

struct State
{
	boost::optional<AnnotationBlock> comment;
	std::string name;
	std::vector<Transition> transitions;
};

struct StateGraph
{
	std::string initial;
	std::vector<State> states;
};

struct Declaration
{
	Type type;
	bool is_array;
	std::string name;
	boost::optional<InitializerExpression> rhs;
};

struct Contract
{
	std::vector<Declaration> variables;
	StateGraph state_graph;
};

} // namespace franca

#endif /* FRANCA_CONTRACT_TYPES_HPP */
