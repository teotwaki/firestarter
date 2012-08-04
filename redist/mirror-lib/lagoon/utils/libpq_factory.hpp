/**
 * @file lagoon/utils/libpq_factory.hpp
 * @brief libpq-based polymorphic factory implementation
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_LIBPQ_FACTORY_1011291729_HPP
#define LAGOON_UTILS_LIBPQ_FACTORY_1011291729_HPP

#include <lagoon/utils/sql_factory.hpp>
#include <mirror/utils/libpq_factory.hpp>

LAGOON_NAMESPACE_BEGIN

using mirror::libpq_fact_data;
using mirror::libpq_fact_result;
using mirror::libpq_fact_db;

typedef sql_fact_builder_templ<
	sql_fact_def_traits<libpq_fact_data>
> libpq_factory_builder;


LAGOON_NAMESPACE_END

#endif //include guard

