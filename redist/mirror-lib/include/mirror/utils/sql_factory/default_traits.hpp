/**
 * .file mirror/utils/sql_factory/default_traits.hpp
 * .brief Default traits template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_DEFAULT_TRAITS_1011291729_HPP
#define MIRROR_UTILS_SQL_FACTORY_DEFAULT_TRAITS_1011291729_HPP

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename Helper, typename Data>
struct sql_fact_def_traits
{
	typedef Helper helper_type;
	typedef Data data_type;
};

MIRROR_NAMESPACE_END

#endif //include guard

