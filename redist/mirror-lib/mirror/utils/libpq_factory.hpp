/**
 * @file mirror/utils/libpq_factory.hpp
 * @brief postgresql/libpq-based factory template for the factory generator
 * utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LIBPQ_FACTORY_1011291729_HPP
#define MIRROR_UTILS_LIBPQ_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sql_factory.hpp>

// libpq factory-related
#include <mirror/utils/libpq_factory/data.hpp>
#include <mirror/utils/libpq_factory/pq_tools.hpp>

MIRROR_NAMESPACE_BEGIN

typedef factory_maker<
	sql_fact_source,
	sql_fact_pool,
	sql_fact_enum,
	sql_fact_def_traits<sql_fact_helper, libpq_fact_data>
> libpq_factory_maker;

typedef factory_maker<
	sql_fact_source,
	sql_fact_pool,
	sql_fact_enum,
	sql_fact_def_traits<sql_fact_quick_helper, libpq_fact_data>
> libpq_quick_factory_maker;

MIRROR_NAMESPACE_END

#endif //include guard

