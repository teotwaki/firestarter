/**
 * @file lagoon/utils/wx_gui_factory/builder.hpp
 * @brief wxWidgets-based GUI polymorphic factory builder
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_BUIDLER_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_BUIDLER_1011291729_HPP

#include <lagoon/polymorph_factory.hpp>
#include <lagoon/utils/auxiliary/fact_builder.hpp>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class wx_gui_factory_builder_templ
 : public aux::common_fact_builder_templ<
	Traits,
	wx_gui_factory_manager,
	wx_gui_factory_composite,
	wx_gui_factory_arrayer,
	wx_gui_factory_suppliers,
	wx_gui_factory_enumerator,
	wx_gui_factory_manufacturer
>{ };

typedef wx_gui_factory_builder_templ<wx_gui_factory_default_traits>
	wx_gui_factory_builder;

LAGOON_NAMESPACE_END

#endif //include guard

