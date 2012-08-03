/**
 * .file mirror/utils/wx_gui_factory/boolean.hpp
 * .brief Implementation of the wx-gui factory template for the bool type
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_BOOL_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_BOOL_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/utils.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>

MIRROR_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_input_gui<bool, SourceTraits>
 : public wx_input_gui_utils<SourceTraits>
{
private:
	wxCheckBox* chb_ctl;
	wx_input_gui_data data;
	struct chbctl_maker
	{
		inline wxCheckBox* operator()(wxWindow* parent) const
		{
			return new wxCheckBox(
				parent,
				wxID_ANY,
				wxEmptyString
			);
		}
	};

public:
	template <class ConstructionInfo>
	wx_input_gui(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info,
		const bool* opt_default = (const bool*)nullptr
	): wx_input_gui_utils<SourceTraits>(parent_data)
	 , chb_ctl(
		this->make_input_unit(
			chbctl_maker(),
			parent_data,
			construction_info
		)
	), data(
		parent_data,
		chb_ctl,
		ConstructionInfo::parameter_index::value,
		SourceTraits::make_func_param_data_id(construction_info)
	)
	{
		if(opt_default) chb_ctl->SetValue(*opt_default);
	}

	void finish(const wx_input_gui_data& parent_data)
	{
		this->finish_input_unit(chb_ctl, parent_data, data);
	}

	inline bool operator()(void) const
	{
		return chb_ctl->GetValue();
	}
};

// Implementation of the state saver for a Checkbox
template <>
struct wx_input_gui_state_saver<wxCheckBox>
{
	static bool save_state(wxCheckBox* widget, wxString& dst)
	{
		assert(widget != nullptr);
		if(widget->Is3State())
		{
			switch(widget->Get3StateValue())
			{
				case wxCHK_CHECKED:
					dst.assign(wxT("true"));
					break;
				case wxCHK_UNCHECKED:
					dst.assign(wxT("false"));
					break;
				case wxCHK_UNDETERMINED:
					dst.assign(wxT("unknown"));
					break;
				default: return false;
			}
		}
		else if(widget->IsChecked())
			dst.assign(wxT("true"));
		else dst.assign(wxT("false"));
		return true;
	}

	static bool load_state(wxCheckBox* widget, const wxString& src)
	{
		assert(widget != nullptr);
		if(src == wxT("true"))
			widget->SetValue(true);
		else if(src == wxT("false"))
			widget->SetValue(false);
		else if(widget->Is3State())
			widget->Set3StateValue(wxCHK_UNDETERMINED);
		else return false;
		return true;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

