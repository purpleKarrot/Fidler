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

#ifndef FRANCA_INITIALIZER_REFLECTION_HPP
#define FRANCA_INITIALIZER_REFLECTION_HPP

#include <fidler/ast/initializer.hpp>
#include <fidler/util/reflect.hpp>

FIDLER_REFLECT(fidler::ast::CompoundInitializer, (elements))
FIDLER_REFLECT(fidler::ast::BracketInitializer, (elements))
FIDLER_REFLECT(fidler::ast::FieldInitializer, (element)(value))
FIDLER_REFLECT(fidler::ast::ElementInitializer, (first)(second))

#endif /* FRANCA_INITIALIZER_REFLECTION_HPP */
