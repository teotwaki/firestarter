/**
 * .file mirror/utils/wx_gui_factory/string.hpp
 * .brief String conversion utilities
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY__1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY__1011291729_HPP

#include <wx/string.h>
#include <wx/strconv.h>
#include <string>

MIRROR_NAMESPACE_BEGIN

inline wxString wx_input_gui_str(const std::string& str)
{
	return wxString(
		str.c_str(),
		wxConvUTF8,
		str.size()
	);
}

MIRROR_NAMESPACE_END

#endif //include guard

