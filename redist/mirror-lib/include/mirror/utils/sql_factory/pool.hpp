/**
 * .file mirror/utils/sql_factory/pool.hpp
 * .brief Existing object pool for the sql-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_POOL_1011291729_HPP
#define MIRROR_UTILS_SQL_FACTORY_POOL_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/unused_type.hpp>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

template <typename Product, typename Traits>
class sql_fact_pool
{
public:
	sql_fact_pool(
		const typename Traits::data_type&,
		unused_type construction_info
	)
	{ }

	void finish(const typename Traits::data_type& parent_data)
	{ }

	inline Product operator()(void)
	{
		assert(!"No values to copy from!");
		return *((Product*)nullptr);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

