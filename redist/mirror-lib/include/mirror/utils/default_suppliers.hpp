/**
 * @file mirror/utils/default_suppliers.hpp
 * @brief Default implementation of the Suppliers parameter
 * for simple factories that do not support copy construction.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_DEFAULT_SUPPLIERS_1011291729_HPP
#define MIRROR_UTILS_DEFAULT_SUPPLIERS_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/mirror_fwd.hpp>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

/** Default implementation of the Factory's Suppliers parameter
 *  for simple factories that do not support copy-construction.
 *
 *  @ingroup factory_generator
 */
template <typename Product, class Unused>
struct default_fact_suppliers
{
	template <typename Param, class ConstrInfo>
	inline default_fact_suppliers(Param, ConstrInfo)
	{ }

	template <typename Param>
	void finish(Param){ }

	inline Product operator()(void) const
	{
		assert(!"Copy construction not supported!");
		return *((Product*)nullptr);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

