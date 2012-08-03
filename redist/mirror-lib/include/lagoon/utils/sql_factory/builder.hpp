/**
 * .file lagoon/utils/sql_factory/builder.hpp
 * .brief SQL-based polymorphic factory builder
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_BUIDLER_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_BUIDLER_1011291729_HPP

#include <lagoon/polymorph_factory.hpp>
#include <lagoon/utils/sql_factory/helper.hpp>
#include <lagoon/utils/auxiliary/fact_builder.hpp>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sql_fact_builder_templ
 : public aux::common_fact_builder_templ<
	Traits,
	sql_fact_manager,
	sql_fact_composite,
	sql_fact_arrayer,
	sql_fact_suppliers,
	sql_fact_enumerator,
	sql_fact_manufacturer
>{ };

LAGOON_NAMESPACE_END

#endif //include guard

