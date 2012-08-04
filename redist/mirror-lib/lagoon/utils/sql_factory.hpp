/**
 * @file lagoon/utils/sql_factory.hpp
 * @brief SQL-based framework polymorphic factory implementation
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_1011291729_HPP

#include <lagoon/config.hpp>
#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/utils.hpp>
#include <lagoon/interfaces.hpp>
#include <lagoon/polymorph_factory.hpp>

// SQL factory-related
#include <lagoon/utils/sql_factory/manager.hpp>
#include <lagoon/utils/sql_factory/composite.hpp>
#include <lagoon/utils/sql_factory/arrayer.hpp>
#include <lagoon/utils/sql_factory/manufacturer.hpp>
#include <lagoon/utils/sql_factory/suppliers.hpp>
#include <lagoon/utils/sql_factory/enumerator.hpp>
#include <lagoon/utils/sql_factory/builder.hpp>

#include <mirror/utils/sql_factory/default_traits.hpp>

LAGOON_NAMESPACE_BEGIN

template <typename Data>
struct sql_fact_def_traits
{
	typedef sql_fact_helper helper_type;
	typedef Data data_type;
};

LAGOON_NAMESPACE_END

#endif //include guard

