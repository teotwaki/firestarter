/**
 * .file lagoon/utils/script_factory/builder.hpp
 * .brief Script-parsing polymorphic factory builder
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_BUIDLER_1011291729_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_BUIDLER_1011291729_HPP

#include <lagoon/polymorph_factory.hpp>
#include <lagoon/utils/auxiliary/fact_builder.hpp>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class script_fact_builder_templ
 : public aux::common_fact_builder_templ<
	Traits,
	script_fact_manager,
	script_fact_composite,
	script_fact_arrayer,
	script_fact_suppliers,
	script_fact_enumerator,
	script_fact_manufacturer
>{ };

LAGOON_NAMESPACE_END

#endif //include guard

