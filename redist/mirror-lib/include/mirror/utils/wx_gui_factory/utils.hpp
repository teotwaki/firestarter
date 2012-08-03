/**
 * .file mirror/utils/wx_gui_factory/utils.hpp
 * .brief Utilities used by the wxWidgets-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_UTILS_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_UTILS_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_class.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>

MIRROR_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_input_gui_utils
{
protected:
	SourceTraits source_traits;
private:
	class InputUnitPanel : public wxPanel
	{
	private:
		typename wx_input_gui_data::weak_ref data_ref;

		void HandleNavigation(wxNavigationKeyEvent& evt)
		{
			wx_input_gui_data unit_data(data_ref);
			if(evt.GetDirection())
				unit_data->focus_next();
			else unit_data->focus_prev();
			evt.StopPropagation();
		}

		void HandleKeyPress(wxKeyEvent& evt)
		{
			switch(evt.GetKeyCode())
			{
			case WXK_RETURN:
			{
				wxNavigationKeyEvent nav_evt;
				nav_evt.SetFromTab(false);
				nav_evt.SetDirection(!evt.ShiftDown());
				nav_evt.SetCurrentFocus(
					static_cast<wxWindow*>(
						evt.GetEventObject()
					)
				);
				this->ProcessEvent(nav_evt);
			}
			case WXK_TAB:
				break;
			default: evt.Skip();
			}
		}
	public:
		InputUnitPanel(const wx_input_gui_data& parent_data)
		 : wxPanel(parent_data->window(), wxID_ANY)
		{ }

		void initialize(const wx_input_gui_data& child_data)
		{
			data_ref = child_data;
			Connect(
				wxEVT_KEY_DOWN,
				wxKeyEventHandler(
					InputUnitPanel::HandleKeyPress
				)
			);
			Connect(
				wxEVT_NAVIGATION_KEY,
				wxNavigationKeyEventHandler(
					InputUnitPanel::HandleNavigation
				)
			);
		}
	}* unit_panel;
private:
	template <class CtrlMaker>
	auto do_make_input_unit(
		CtrlMaker make_ctrl,
		const wx_input_gui_data& parent_data,
		const wxString& func_param_label,
		const wxString& func_param_hint
	) -> decltype(make_ctrl(parent_data->window()))
	{
		wxBoxSizer* unit_sizer = new wxBoxSizer(wxHORIZONTAL);
		// add the label
		unit_sizer->Add(
			new wxStaticText(
				unit_panel,
				wxID_ANY,
				func_param_label,
				wxDefaultPosition,
				wxDefaultSize,
				wxALIGN_RIGHT
			),
			1,
			wxEXPAND
		);
		// add a spacer
		unit_sizer->AddSpacer(source_traits.major_spacer_width());
		// use the maker to make the input control
		auto input_ctrl = make_ctrl(unit_panel);
		// make and set the tool tip for the input control
		input_ctrl->SetToolTip(func_param_hint);
		// add it to the input unit's sizer
		unit_sizer->Add(input_ctrl, 2, wxEXPAND);
		// add one more spacer
		unit_sizer->AddSpacer(source_traits.minor_spacer_width());
		//
		// there must be no other sizer assigned to the unit_panel
		// this effectivelly means that this function can be called
		// only once per utils instance
		assert(unit_panel->GetSizer() == nullptr);
		// assign the sizer to the unit panel
		unit_panel->SetSizer(unit_sizer);
		//
		// NOTE: the input control will be added to the parent sizer
		// in the finish_input_ctrl member function because this
		// (make_input_ctrl) function can be called in a wrong
		// order, but the finish function is called always in
		// the same order as is the order of the parameters
		// in the function / constructor declaration
		//
		// return the input widget
		return input_ctrl;
	}

protected:
	template <class CtrlMaker, class ConstructionInfo>
	auto make_input_unit(
		CtrlMaker make_ctrl,
		const wx_input_gui_data& parent_data,
		ConstructionInfo constr_info
	) -> decltype(make_ctrl(parent_data->window()))
	{
		return do_make_input_unit(
			make_ctrl,
			parent_data,
			source_traits.make_func_param_label(constr_info),
			source_traits.make_func_param_hint(constr_info)
		);
	}

	template <typename InputCtrl>
	void finish_input_unit(
		InputCtrl* input_ctrl,
		const wx_input_gui_data& parent_data,
		const wx_input_gui_data& child_data
	)
	{
		// get the unit panel from the input control
		InputUnitPanel* unit_panel =
			static_cast<InputUnitPanel*>(input_ctrl->GetParent());
		assert(unit_panel != nullptr);
		// initialize it with the child data
		unit_panel->initialize(child_data);
		// add the whole thing to the parent sizer
		parent_data->sizer()->Add(
			unit_panel,
			0,
			wxEXPAND
		);
	}

	wx_input_gui_utils(const wx_input_gui_data& data)
	 : source_traits(data)
	 , unit_panel(new InputUnitPanel(data))
	{ }

};

MIRROR_NAMESPACE_END

#endif //include guard

