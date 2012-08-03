/**
 * @file lagoon/utils/wx_gui_factory/default_traits.hpp
 * @brief wxWidgets-based GUI polymorphic factory default traits
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_DEFAULT_TRAITS_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_DEFAULT_TRAITS_1011291729_HPP

#include <wx/choicebk.h>

LAGOON_NAMESPACE_BEGIN

class wx_gui_factory_default_traits
{
private:
	static inline wxString translated(const std::string& str)
	{
		return wxGetTranslation(wxString(
			str.c_str(),
			wxConvUTF8,
			str.size()
		));
	}

	static inline wxString translated(const wxString& str)
	{
		return wxGetTranslation(str);
	}
public:
	typedef wxChoicebook base_book_ctrl;

	static wxString get_message(const wxString& default_message)
	{
		return translated(default_message);
	}

	static wxString make_constructor_label(
		const shared<meta_constructor>& constructor
	)
	{
		wxString label(translated(constructor->base_name()));
		label.append(wxT("("));
		auto params = constructor->parameters();
		while(!params.empty())
		{
			label.append(translated(params.front()->base_name()));
			params.step_front();
			if(!params.empty())
				label.append(wxT(","));
		}
		label.append(wxT(")"));
		return translated(label);
	}

	static wxString make_product_label(const shared<meta_type>& product)
	{
		return translated(product->base_name());
	}

	static wxString make_func_param_label(const shared<meta_parameter>& param)
	{
		return translated(param->base_name());
	}

	static wxString make_enum_value_label(
		const shared<meta_enum>& enumeration,
		int index
	)
	{
		return translated(enumeration->value_name(index));
	}

	static wxString make_func_param_hint(const shared<meta_parameter>& param)
	{
		wxString label;
		label.append(translated(param->type()->full_name()));
		label.append(wxT(" "));
		label.append(translated(param->base_name()));
		return label;
	}

	static int padding_left(void) { return 8; }
	static int padding_top(void) { return 8; }
	static int padding_right(void) { return 16; }
	static int padding_bottom(void) { return 16; }

	static int minor_spacer_width(void) { return 8; }
	static int major_spacer_width(void) { return 16; }
};

LAGOON_NAMESPACE_END

#endif //include guard

