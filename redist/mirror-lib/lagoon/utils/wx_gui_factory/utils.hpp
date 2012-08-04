/**
 * @file lagoon/utils/wx_gui_factory/utils.hpp
 * @brief wxWidgets-based GUI polymorphic factory utilities
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_UTILS_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_UTILS_1011291729_HPP

#include <lagoon/utils/wx_gui_factory/data.hpp>

LAGOON_NAMESPACE_BEGIN

class wx_gui_factory_utils
{
private:
	static wxWindow* get_window(wx_gui_factory_data* data)
	{
		if(data) return data->window;
		else return nullptr;
	}

	static wxSizer* get_sizer(wx_gui_factory_data* data)
	{
		if(data) return data->sizer;
		else return nullptr;
	}
public:
	static wx_gui_factory_data* unwrap(raw_ptr data)
	{
		return raw_cast<wx_gui_factory_data*>(data);
	}

	static wxWindow* unwrap_window(raw_ptr data)
	{
		return get_window(unwrap(data));
	}

	static wxSizer* unwrap_sizer(raw_ptr data)
	{
		return get_sizer(unwrap(data));
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

