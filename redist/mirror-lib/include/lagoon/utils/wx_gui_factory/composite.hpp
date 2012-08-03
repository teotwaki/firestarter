/**
 * @file lagoon/utils/wx_gui_factory/composite.hpp
 * @brief wxWidgets-based GUI polymorphic factory composite
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_COMPOSITE_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_COMPOSITE_1011291729_HPP

#include <cassert>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_gui_factory_composite
 : public wx_gui_factory_utils
 , public polymorph_factory_composite
{
private:
	SourceTraits source_traits;

	wxDialog* dialog;
	wx_gui_factory_data unit_data;

	void make_dialog_buttons(wxSizer* dialog_sizer)
	{
		assert(dialog != nullptr);
		assert(dialog_sizer != nullptr);
		// standard button set
		wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
		button_sizer->AddStretchSpacer();
		button_sizer->AddSpacer(source_traits.major_spacer_width());
		button_sizer->Add(new wxButton(dialog, wxID_CLEAR));
		button_sizer->AddSpacer(source_traits.major_spacer_width());
		button_sizer->Add(new wxButton(dialog, wxID_OK));
		button_sizer->AddSpacer(source_traits.minor_spacer_width());
		button_sizer->Add(new wxButton(dialog, wxID_CANCEL));
		// add the button sizer
		dialog_sizer->AddSpacer(source_traits.major_spacer_width());
		dialog_sizer->Add(button_sizer, 0, wxEXPAND);
	}
public:
	wx_gui_factory_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): dialog(
		new wxDialog(
			this->unwrap_window(parent_data),
			wxID_ANY,
			source_traits.make_product_label(product),
			wxDefaultPosition,
			wxDefaultSize,
			wxDEFAULT_DIALOG_STYLE |
			wxRESIZE_BORDER
		)
	), unit_data(dialog)
	{
		wxBoxSizer* outer_sizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* inner_sizer = new wxBoxSizer(wxVERTICAL);
		outer_sizer->AddSpacer(source_traits.padding_left());
		inner_sizer->AddSpacer(source_traits.padding_top());
		inner_sizer->Add(unit_data.sizer, 1, wxEXPAND);
		inner_sizer->AddSpacer(source_traits.padding_bottom());
		outer_sizer->Add(inner_sizer, 1, wxEXPAND);
		outer_sizer->AddSpacer(source_traits.padding_right());
		dialog->SetSizer(outer_sizer);
	}

	wx_gui_factory_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): dialog(nullptr)
	 , unit_data(
		new wxPanel(
			this->unwrap_window(parent_data),
			wxID_ANY
		),
		source_traits.make_func_param_label(param)
	)
	{
		unit_data.window->SetToolTip(
			source_traits.make_func_param_hint(param)
		);
		unit_data.window->SetSizer(unit_data.sizer);
	}

	~wx_gui_factory_composite(void)
	{
		if(dialog != nullptr) dialog->Destroy();
	}

	void finish(raw_ptr parent_data)
	{
		if(dialog == nullptr)
		{
			wxSizer* sizer = this->unwrap_sizer(parent_data);
			assert(sizer != nullptr);
			sizer->Add(unit_data.window, 0, wxEXPAND);
		}
		else
		{
			make_dialog_buttons(unit_data.sizer);
			unit_data.sizer->SetSizeHints(dialog);
			dialog->SetMinSize(dialog->GetSize());
		}
		unit_data.window->SetExtraStyle(
			unit_data.window->GetExtraStyle() |
			wxWS_EX_VALIDATE_RECURSIVELY
		);
	}

	raw_ptr data(void)
	{
		return raw_ptr(&unit_data);
	}

	void on_create(void)
	{
		if(dialog != nullptr)
		{
			if(dialog->ShowModal() != wxID_OK)
			{
				throw std::runtime_error(
					"Construction aborted by user"
				);
			}
		}
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

