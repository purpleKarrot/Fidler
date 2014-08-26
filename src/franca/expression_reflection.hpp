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

#ifndef FRANCA_EXPRESSION_REFLECTION_HPP
#define FRANCA_EXPRESSION_REFLECTION_HPP

#include <fidler/ast/expression.hpp>
#include <fidler/util/reflect.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

FIDLER_REFLECT(fidler::ast::Expression, (left)(right))
FIDLER_REFLECT(fidler::ast::NullCoalescingExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::LogicalOrExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::LogicalAndExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::InclusiveOrExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::ExclusiveOrExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::AndExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::EqualityExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::RelationalExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::ShiftExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::AdditiveExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::MultiplicativeExpression, (left)(right))
FIDLER_REFLECT(fidler::ast::PrefixExpression, (op)(expr))
FIDLER_REFLECT(fidler::ast::PostfixExpression, (base)(postfix))
FIDLER_REFLECT(fidler::ast::PostfixExpression::MemberAccess, (member))
FIDLER_REFLECT(fidler::ast::PostfixExpression::Subscript, (value))
FIDLER_REFLECT(fidler::ast::Identifier, (name))

#endif /* FRANCA_EXPRESSION_REFLECTION_HPP */
