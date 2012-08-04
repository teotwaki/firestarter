/**
 * @file lagoon/utils/script_factory.hpp
 * @brief Script-parsing framework polymorphic factory implementation
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_1011291729_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_1011291729_HPP

#include <lagoon/config.hpp>
#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/utils.hpp>
#include <lagoon/interfaces.hpp>
#include <lagoon/polymorph_factory.hpp>

#include <mirror/utils/script_factory.hpp>
// Script-parsing factory-related
#include <lagoon/utils/script_factory/utils.hpp>
#include <lagoon/utils/script_factory/manager.hpp>
#include <lagoon/utils/script_factory/composite.hpp>
#include <lagoon/utils/script_factory/arrayer.hpp>
#include <lagoon/utils/script_factory/manufacturer.hpp>
#include <lagoon/utils/script_factory/suppliers.hpp>
#include <lagoon/utils/script_factory/enumerator.hpp>
#include <lagoon/utils/script_factory/builder.hpp>

#include <mirror/utils/script_factory/default_traits.hpp>

LAGOON_NAMESPACE_BEGIN

using mirror::script_fact_def_traits;
using mirror::script_factory_input;
using mirror::c_str_script_factory_input;

typedef script_fact_builder_templ<
	script_fact_def_traits<std::string::const_iterator>
> script_factory_builder;

typedef script_fact_builder_templ<
	script_fact_def_traits<const char*>
> c_str_script_factory_builder;

LAGOON_NAMESPACE_END

#endif //include guard

