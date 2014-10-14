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

#ifndef FIDLER_AST_CONTRACT_HPP
#define FIDLER_AST_CONTRACT_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <fidler/ast/typesystem.hpp>
#include <fidler/ast/expression.hpp>
#include <fidler/ast/initializer.hpp>

namespace fidler
{
namespace ast
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

struct Assignment
{
	enum Operator
	{
		assign,
		mul_assign,
		div_assign,
		mod_assign,
		plus_assign,
		minus_assign,
		lshift_assign,
		rshift_assign,
		and_assign,
		xor_assign,
		or_assign
	};

	std::string lhs;
	Operator op;
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

struct Block
{
	std::vector<Statement> statements;
};

struct Transition
{
	Trigger trigger;
	std::string event;
	boost::optional<Expression> guard;
	std::string to;
	boost::optional<Block> action;
};

struct State
{
	std::string comment;
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
	bool is_array = false;
	std::string name;
	boost::optional<InitializerExpression> rhs;
};

struct Contract
{
	std::vector<Declaration> variables;
	StateGraph state_graph;
};

} // namespace ast
} // namespace fidler

#endif /* FIDLER_AST_CONTRACT_HPP */
