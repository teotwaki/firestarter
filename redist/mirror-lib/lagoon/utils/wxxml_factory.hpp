/**
 * @file lagoon/utils/wxxml_factory.hpp
 * @brief RapidXML-based polymorphic factory implementation
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WXXML_FACTORY_1011291729_HPP
#define LAGOON_UTILS_WXXML_FACTORY_1011291729_HPP

#include <lagoon/config.hpp>
#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/utils.hpp>
#include <lagoon/interfaces.hpp>

#include <mirror/utils/wxxml_factory.hpp>
#include <lagoon/utils/sdn_factory.hpp>

LAGOON_NAMESPACE_BEGIN

using mirror::wxxml_factory_input;

typedef sdn_fact_builder_templ<mirror::sdn_fact_wxxml_traits>
	wxxml_factory_builder;

LAGOON_NAMESPACE_END

#endif //include guard

