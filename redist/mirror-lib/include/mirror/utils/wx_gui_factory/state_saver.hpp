/**
 * .file mirror/utils/wx_gui_factory/state_saver.hpp
 * .brief Helper used for saving the state of a wxWidget to and from a string
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_STATE_SAVER_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_STATE_SAVER_1011291729_HPP

#include <wx/wx.h>

MIRROR_NAMESPACE_BEGIN

// Default do-nothing implementation of the state saver for the wxWidgets-based
// factory generator template
template <class WxWidget>
struct wx_input_gui_state_saver
{
	static bool save_state(WxWidget* widget, wxString& dst)
	{
		return false;
	}

	static bool load_state(WxWidget* widget, const wxString& src)
	{
		return false;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

