/**
 * @file lagoon/utils/wx_gui_factory/data.hpp
 * @brief wxWidgets-based GUI polymorphic factory data
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_DATA_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_DATA_1011291729_HPP

#include <wx/wx.h>
#include <wx/sizer.h>

LAGOON_NAMESPACE_BEGIN

struct wx_gui_factory_data
{
	wxWindow* const window;
	wxSizer* const sizer;

	inline wx_gui_factory_data(wxWindow* wnd, wxSizer* szr)
	 : window(wnd)
	 , sizer(szr)
	{ }

	inline wx_gui_factory_data(wxDialog* dlg)
	 : window(dlg)
	 , sizer(new wxBoxSizer(wxVERTICAL))
	{ }

	inline wx_gui_factory_data(
		wxPanel* panel,
		const wxString& title
	): window(panel)
	 , sizer(new wxStaticBoxSizer(wxVERTICAL, panel, title))
	{ }

	inline wx_gui_factory_data(wxPanel* panel)
	 : window(panel)
	 , sizer(new wxBoxSizer(wxHORIZONTAL))
	{ }
};

LAGOON_NAMESPACE_END

#endif //include guard

