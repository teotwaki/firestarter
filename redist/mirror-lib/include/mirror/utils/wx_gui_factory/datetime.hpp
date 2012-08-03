/**
 * .file mirror/utils/wx_gui_factory/datetime.hpp
 * .brief Implementation of the wx-gui factory template for the std::tm struct
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_DATETIME_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_DATETIME_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/utils.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/datectrl.h>

#include <ctime>

MIRROR_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_input_gui<std::tm, SourceTraits>
 : public wx_input_gui_utils<SourceTraits>
{
private:
	//TODO a time picker component is missing
	wxDatePickerCtrl* dt_ctl;
	wx_input_gui_data data;
	struct dtctl_maker
	{
		inline wxDatePickerCtrl* operator()(wxWindow* parent) const
		{
			return new wxDatePickerCtrl(
				parent,
				wxID_ANY,
				wxDefaultDateTime
			);
		}
	};

	static inline std::tm to_tm(const wxDateTime& datetime)
	{
		std::tm result;
		result.tm_sec  = datetime.GetSecond();
		result.tm_min  = datetime.GetMinute();
		result.tm_hour = datetime.GetHour();
		result.tm_mday = datetime.GetDay();
		result.tm_mon  = datetime.GetMonth();
		result.tm_year = datetime.GetYear();
		result.tm_wday = datetime.GetWeekDay();
		result.tm_yday = datetime.GetDayOfYear();
		result.tm_isdst= datetime.IsDST();
		return result;
	}

public:
	template <class ConstructionInfo>
	wx_input_gui(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info,
		const std::tm* opt_default = (const std::tm*)nullptr
	): wx_input_gui_utils<SourceTraits>(parent_data)
	 , dt_ctl(
		this->make_input_unit(
			dtctl_maker(),
			parent_data,
			construction_info
		)
	), data(
		parent_data,
		dt_ctl,
		ConstructionInfo::parameter_index::value,
		SourceTraits::make_func_param_data_id(construction_info)
	)
	{
		if(opt_default) dt_ctl->SetValue(wxDateTime(*opt_default));
	}

	void finish(const wx_input_gui_data& parent_data)
	{
		this->finish_input_unit(dt_ctl, parent_data, data);
	}

	inline std::tm operator()(void) const
	{
		return to_tm(dt_ctl->GetValue());
	}
};

// Implementation of the state saver for the DatePicker widget
template <>
struct wx_input_gui_state_saver<wxDatePickerCtrl>
{
	static bool save_state(wxDatePickerCtrl* widget, wxString& dst)
	{
		assert(widget != nullptr);
		dst.assign(widget->GetValue().Format());
		return true;
	}

	static bool load_state(wxDatePickerCtrl* widget, const wxString& src)
	{
		assert(widget != nullptr);
		wxDateTime datetime;
		if(datetime.ParseFormat(src.c_str()) != nullptr)
		{
			widget->SetValue(datetime);
			return true;
		}
		else return false;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

