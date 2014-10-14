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

#ifndef FRANCA_MODEL_REFLECTION_HPP
#define FRANCA_MODEL_REFLECTION_HPP

#include <fidler/ast/model.hpp>
#include <fidler/util/reflect.hpp>

FIDLER_REFLECT(fidler::ast::Version,
	(major)
	(minor)
)

FIDLER_REFLECT(fidler::ast::Attribute,
	(comment)
	(type)
	(is_array)
	(name)
	(read_only)
	(no_subscriptions)
)

FIDLER_REFLECT(fidler::ast::Argument,
	(type)
	(is_array)
	(name)
	(init)
)

FIDLER_REFLECT(fidler::ast::Model,
	(name)
	(imports)
	(type_collections)
	(interfaces)
)

FIDLER_REFLECT(fidler::ast::TypeCollection,
	(comment)
	(name)
	(version)
	(types)
	(constants)
)

FIDLER_REFLECT(fidler::ast::Interface,
	(comment)
	(name)
	(base)
	(managed_interfaces)
	(version)
	(attributes)
	(methods)
	(broadcasts)
	(contract)
	(types)
	(constants)
)

FIDLER_REFLECT(fidler::ast::Method,
	(comment)
	(name)
	(fire_and_forget)
	(in_args)
	(out_args)
	(error)
)

FIDLER_REFLECT(fidler::ast::Broadcast,
	(comment)
	(name)
	(selective)
	(out_args)
)

FIDLER_REFLECT(fidler::ast::ConstantDef,
	(type)
	(is_array)
	(name)
	(rhs)
)

FIDLER_REFLECT(fidler::ast::Enumerator,
	(name)
	(value)
)

#endif /* FRANCA_MODEL_REFLECTION_HPP */
