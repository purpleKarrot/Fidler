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

#include "get_element.hpp"
#include "context.hpp"
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include "../franca/model_reflection.hpp"
#include <cxxabi.h>
#include <iostream>

namespace mustache
{

template<typename Seq, int N>
struct member_name: boost::fusion::extension::struct_member_name<Seq, N>
{
};

template<typename Seq, int N>
struct member_name<Seq const, N>: member_name<Seq, N>
{
};

template<int N>
struct get_element_impl
{
	template<typename Sequence>
	static Context call(Sequence &seq, std::string const &name, Context const* parent)
	{
		auto ret = get_element_impl<N - 1>::call(seq, name, parent);
		if (!ret && member_name<Sequence, N - 1>::call() == name)
		{
			ret = Context(boost::fusion::at_c<N - 1>(seq), parent);
		}
		return ret;
	}
};

template<>
struct get_element_impl<0>
{
	template<typename Sequence>
	static Context call(Sequence &seq, std::string const &name, Context const* parent)
	{
		return Context(parent);
	}
};


template<typename Sequence>
Context get_element_(Sequence const& seq, std::string const& name, Context const* parent, boost::mpl::true_)
{
	using namespace boost::fusion;
	return get_element_impl<result_of::size<Sequence>::value>::call(seq, name, parent);
}

template<typename T>
Context get_element_(T const& v, std::string const& name, Context const* parent, boost::mpl::false_)
{
	std::cout
		<< "STUB: '"
		<< abi::__cxa_demangle(typeid(T).name(), 0, 0, 0)
		<< "' is not reflected.\n"
		;
	return Context(parent);
}

template<typename Sequence>
Context get_element(Sequence const& seq, std::string const& name, Context const* parent)
{
	return get_element_(seq, name, parent, typename boost::fusion::traits::is_sequence<Sequence>::type());
}

using namespace fidler::ast;
template Context get_element<bool>(bool const&, std::string const&, Context const* parent);
template Context get_element<Model>(Model const&, std::string const&, Context const* parent);

} // namespace mustache
