/**
 * @file lagoon/utils/wx_gui_factory/enumerator.hpp
 * @brief wxWidgets-based GUI polymorphic factory suppliers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_ENUMERATOR_1102011514_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_ENUMERATOR_1102011514_HPP

#include <cassert>
#include <wx/combobox.h>
#include <mirror/utils/wx_gui_factory/enum_validator.hpp>

LAGOON_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_gui_factory_enumerator
 : public wx_gui_factory_utils
 , public polymorph_factory_enumerator
{
private:
	SourceTraits source_traits;
	wx_gui_factory_data unit_data;
	shared<meta_enum> enumeration;
	wxComboBox* combo_ctl;

	wxComboBox* make_combo(void) const
	{
		wxArrayString item_names;
		for(int i=0,n=enumeration->size(); i!=n; ++i)
		{
			item_names.Add(
				source_traits.make_enum_value_label(
					enumeration,
					i
				)
			);
		}
		wxString message(
			source_traits.get_message(
				wxT("A value must be selected")
			)
		);
		wxComboBox* result = new wxComboBox(
			unit_data.window,
			wxID_ANY,
			wxEmptyString,
			wxDefaultPosition,
			wxDefaultSize,
			item_names,
			wxCB_READONLY,
			mirror::aux::EnumValidator(
				message,
				enumeration->size()
			)
		);
		return result;
	}
public:
	wx_gui_factory_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): unit_data(new wxPanel(this->unwrap_window(parent_data), wxID_ANY))
	 , enumeration(param->type().as<meta_enum>())
	 , combo_ctl(make_combo())
	{
		unit_data.window->SetToolTip(
			source_traits.make_func_param_hint(param)
		);
		wxStaticText* label = new wxStaticText(
			unit_data.window,
			wxID_ANY,
			source_traits.make_func_param_label(param)
		);
		unit_data.sizer->Add(label, 1, wxEXPAND);
		unit_data.sizer->AddSpacer(source_traits.minor_spacer_width());
		unit_data.sizer->Add(combo_ctl, 2, wxEXPAND);
		unit_data.window->SetSizer(unit_data.sizer);
	}

	void finish(raw_ptr parent_data)
	{
		wxSizer* sizer = this->unwrap_sizer(parent_data);
		assert(sizer != nullptr);
		sizer->Add(unit_data.window, 0, wxEXPAND);
	}

	int create(void)
	{
		return enumeration->value(combo_ctl->GetCurrentSelection());
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

