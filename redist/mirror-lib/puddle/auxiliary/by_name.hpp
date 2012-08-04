/**
 * .file puddle/auxiliary/by_name.hpp
 * .brief Implementation of the by_name functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_BY_NAME_1103230901_HPP
#define PUDDLE_AUXILIARY_BY_NAME_1103230901_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/wrap.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <class MetaClass>
struct by_name_scope_transf
{
	struct type
	{
		template <typename T>
		type(const T&){ }
	};
};

template <class MetaVariable>
struct by_name_memvar_transf
{
	struct type
	{
		template <class TransfClass>
		type(TransfClass&, int)
		{ }

		typename wrap<MetaVariable>::type
		operator()(void) const
		{
			return typename wrap<MetaVariable>::type();
		}
	};
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

