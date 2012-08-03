/**
 * .file mirror/utils/wx_gui_factory/dialog_interface.hpp
 * .brief Interface for the factory dialog
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_DIALOG_INTERFACE_1102101118_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_DIALOG_INTERFACE_1102101118_HPP

#include <wx/window.h>
#include <wx/string.h>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <typename Product>
struct wx_gui_factory_dialog_intf
{
	virtual ~wx_gui_factory_dialog_intf(void)
	{ }

	virtual Product* new_(void) = 0;
	virtual Product create(void) = 0;
};

template <typename Product, class SourceTraits>
wx_gui_factory_dialog_intf<Product>* make_wx_gui_factory_dialog(
	wxWindow* parent,
	const wxString& caption
);

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

