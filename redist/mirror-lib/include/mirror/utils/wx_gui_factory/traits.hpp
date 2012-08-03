/**
 * .file mirror/utils/wx_gui_factory/traits.hpp
 * .brief Basic widget property definitions like margins, etc.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_TRAITS_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_TRAITS_1011291729_HPP

#include <mirror/utils/wx_gui_factory/data.hpp>

MIRROR_NAMESPACE_BEGIN

template <class ... SubTraits>
struct wx_input_gui_traits_templ
 : public SubTraits ...
{
	wx_input_gui_traits_templ(const wx_input_gui_data& data)
	 : SubTraits(data)...
	{ }
};

MIRROR_NAMESPACE_END

#endif //include guard

