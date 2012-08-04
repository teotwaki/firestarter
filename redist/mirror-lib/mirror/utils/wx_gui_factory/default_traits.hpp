/**
 * .file mirror/utils/wx_gui_factory/default_traits.hpp
 * .brief Default trait definitions
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_DEFAULT_TRAITS_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_DEFAULT_TRAITS_1011291729_HPP

#include <mirror/utils/wx_gui_factory/string.hpp>
#include <mirror/utils/wx_gui_factory/traits.hpp>
#include <mirror/utils/unused_type.hpp>

#include <wx/wx.h>
#include <wx/strconv.h>
#include <wx/choicebk.h>

class wxAuiNotebook;

MIRROR_NAMESPACE_BEGIN

template <
	class BaseBookCtrl,
	size_t PaddingLeft,
	size_t PaddingTop,
	size_t PaddingRight,
	size_t PaddingBottom,
	size_t MajorSpacerWidth,
	size_t MinorSpacerWidth
>
struct wx_input_gui_widget_traits_templ
{
	typedef BaseBookCtrl base_book_ctrl;

	wx_input_gui_widget_traits_templ(const wx_input_gui_data&)
	{ }

	static inline wxCoord padding_left(void)
	{
		return PaddingLeft;
	}

	static inline wxCoord padding_top(void)
	{
		return PaddingTop;
	}

	static inline wxCoord padding_right(void)
	{
		return PaddingRight;
	}

	static inline wxCoord padding_bottom(void)
	{
		return PaddingBottom;
	}

	static inline wxCoord major_spacer_width(void)
	{
		return MajorSpacerWidth;
	}

	static inline wxCoord minor_spacer_width(void)
	{
		return MinorSpacerWidth;
	}

	// Helper function for getting the correct sizer proportion of this
	// input unit based on the type of the selected BookCtrl which
	// is used by the manager
	static inline int get_default_size_proportion(...)
	{
		return 0;
	}

	static inline int get_default_size_proportion(wxAuiNotebook*)
	{
		return 1;
	}

	static inline int default_size_proportion(void)
	{
		return get_default_size_proportion(
			(BaseBookCtrl*)nullptr
		);
	}
};

typedef wx_input_gui_widget_traits_templ<
	wxChoicebook,
	8, 8, 8, 8, 8, 4
> wx_input_gui_default_widget_traits;

class wx_input_gui_default_text_traits
{
private:
	static inline wxString translated(const std::string& str)
	{
		return wxGetTranslation(wx_input_gui_str(str));
	}

	static inline wxString translated(const wxString& str)
	{
		return wxGetTranslation(str);
	}

	struct param_list_maker
	{
		wxString& str_ref;
		bool first_param;

		param_list_maker(wxString& _ref)
		 : str_ref(_ref)
		 , first_param(true)
		{ }

		template <typename MetaVariable>
		inline void operator()(MetaVariable param)
		{
			// if its not the first parameter
			// prepend the comma
			if(first_param) first_param = false;
			else str_ref.append(wxT(", "));
			// append the parameter name
			str_ref.append(translated(param.base_name()));
		}
	};

public:
	wx_input_gui_default_text_traits(const wx_input_gui_data&)
	{ }

	static wxString make_root_data_id(void)
	{
		return wxT("new");
	}

	static wxString make_helper_data_id(void)
	{
		return wxT("helper");
	}

	static wxString make_manager_data_id(void)
	{
		return wxT("constructors");
	}

	template <class MetaClass, class ConstructorIndex>
	static wxString make_constructor_data_id(
		MetaClass,
		ConstructorIndex ci
	)
	{
		wxString result;
		result.Printf(
			wxT("%s_%d"),
			MetaClass::base_name().c_str(),
			int(ConstructorIndex::value)
		);
		return result;
	}


	template <class ConstructionInfo>
	static inline wxString make_func_param_data_id(ConstructionInfo)
	{
		return wx_input_gui_str(ConstructionInfo::parameter::base_name());
	}

	template <class MetaClass, class ConstructorIndex>
	static wxString make_constructor_label(
		MetaClass,
		ConstructorIndex ci
	)
	{
		wxString label(translated(MetaClass::base_name()));
		label.append(wxT("("));
		typedef typename parameters<
			typename mp::at<
				constructors<MetaClass>,
				ConstructorIndex
			>::type
		>::type meta_fn_params;
		mp::for_each<meta_fn_params>(param_list_maker(label));
		label.append(wxT(")"));
		return translated(label);
	}

	template <class ConstructionInfo>
	static inline wxString make_func_param_label(ConstructionInfo)
	{
		return translated(ConstructionInfo::parameter::base_name());
	}

	template <class ConstructionInfo>
	static inline wxString make_func_param_hint(ConstructionInfo)
	{
		typedef typename ConstructionInfo::parameter param;
		wxString result(translated(param::type::full_name()));
		result.append(wxT(": "));
		result.append(translated(param::full_name()));
		return result;
	}

	static inline wxString get_value_regex_pattern(
		const wxString& sid,
		const wxString& fallback,
		unused_type construction_info
	)
	{
		// try to "translate" the pattern by using a string id
		wxString result(translated(sid));
		// if there is no translation, return the fallback
		if(result == sid) return fallback;
		else return result;
	}

	template <class MetaEnum>
	static inline wxString get_enum_value_label(
		MetaEnum,
		int index
	)
	{
		return translated(MetaEnum::value_name(index));
	}

	static inline wxString get_message(
		const wxString& default_message
	)
	{
		return translated(default_message);
	}

	static wxString get_erase_label(void)
	{
		return translated(wxString(wxT("Erase")));
	}

	static wxString get_append_label(void)
	{
		return translated(wxString(wxT("Append")));
	}

	template <class ConstructionInfo>
	static wxString get_invalid_value_message(
		const wxString& default_message,
		ConstructionInfo construction_info
	)
	{
		wxString msg(get_message(default_message));
		msg.append(wxT(" ("));
		msg.append(make_func_param_label(construction_info));
		msg.append(wxT(")"));
		return msg;
	}
};

typedef wx_input_gui_traits_templ<
 wx_input_gui_default_widget_traits,
 wx_input_gui_default_text_traits
> wx_input_gui_default_traits;

MIRROR_NAMESPACE_END

#endif //include guard

