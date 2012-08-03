/**
 * @file lagoon/utils/wx_gui_factory/manufacturer.hpp
 * @brief wxWidgets-based GUI polymorphic factory manufacturers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_MANUFACTURER_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_MANUFACTURER_1011291729_HPP

#include <cassert>
#include <wx/strconv.h>
#include <wx/datectrl.h>
#include <mirror/utils/wx_gui_factory/regex_validator.hpp>

LAGOON_NAMESPACE_BEGIN

// Fallback implementation of the factory helper
struct wx_gui_factory_hlpr_flbk
{
	// This class does not create an input control
	// It creates a "N/A" label instead
	static wxStaticText* init_input_ctl(wxWindow* parent)
	{
		return new wxStaticText(
			parent,
			wxID_ANY,
			wxT("N/A")
		);
	}
};

// Fallback for concrete product types is to do default construction
template <class Product>
struct wx_gui_factory_hlpr_flbk_tpl
 : public wx_gui_factory_hlpr_flbk
{
	static Product get(void)
	{
		return Product();
	}
};

// All product types that do not have a special helper use the fallback
template <class Product>
struct wx_gui_fact_hlpr
{
	typedef wx_gui_factory_hlpr_flbk_tpl<Product> type;
};

// Base class for concrete helpers
// This class holds and manages the initialization of the input widget
template <class Widget>
class wx_gui_fact_hlpr_base
{
private:
	Widget* input_ctl;
protected:
	wx_gui_fact_hlpr_base(void)
	 : input_ctl(nullptr)
	{ }

	Widget* init_input(Widget* ctl)
	{
		assert(input_ctl == nullptr);
		input_ctl = ctl;
		assert(input_ctl != nullptr);
		return input_ctl;
	}

	Widget& input(void)
	{
		assert(input_ctl != nullptr);
		return *input_ctl;
	}

};

// Factory helper for the boolean type using a check box
class wx_gui_fact_hlpr_bool
 : public wx_gui_fact_hlpr_base<wxCheckBox>
{
public:
	wxWindow* init_input_ctl(wxWindow* parent)
	{
		return init_input(
			new wxCheckBox(
				parent,
				wxID_ANY,
				wxEmptyString
			)
		);
	}

	bool get(void)
	{
		return input().GetValue();
	}
};

template <> struct wx_gui_fact_hlpr<bool>
{
	typedef wx_gui_fact_hlpr_bool type;
};

// Base class for RegEx-validated factory helpers
// This class uses a text control with a regex validator
// Derived classes define the method of conversion from
// string to the resulting type
template <class ValidTraits>
class wx_gui_fact_hlpr_regex
 : public wx_gui_fact_hlpr_base<wxTextCtrl>
{
private:
	ValidTraits valid_traits;
public:
	wxWindow* init_input_ctl(wxWindow* parent)
	{
		return init_input(
			new wxTextCtrl(
				parent,
				wxID_ANY,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				0,
				mirror::aux::RegExValidator(
					valid_traits.pattern(),
					valid_traits.message()
				)
			)
		);
	}
};

// regular expression getter for integral types
struct wx_gui_factory_regex_int
{
	static wxString pattern(void)
	{
		return wxT("^[+-]?[[:digit:]]+$");
	}

	static wxString message(void)
	{
		return wxT("Invalid integer value");
	}
};

// Factory helper for integral product types
struct wx_gui_fact_hlpr_int
 : public wx_gui_fact_hlpr_regex<wx_gui_factory_regex_int>
{
	long get(void)
	{
		long result;
		if(!input().GetValue().ToLong(&result))
			throw std::bad_cast();
		return result;
	}
};

template <> struct wx_gui_fact_hlpr<short int>
{
	typedef wx_gui_fact_hlpr_int type;
};

template <> struct wx_gui_fact_hlpr<int>
{
	typedef wx_gui_fact_hlpr_int type;
};

template <> struct wx_gui_fact_hlpr<long int>
{
	typedef wx_gui_fact_hlpr_int type;
};

// regular expression getter for unsigned integral types
struct wx_gui_factory_regex_uint
{
	static wxString pattern(void)
	{
		return wxT("^[+]?[[:digit:]]+$");
	}

	static wxString message(void)
	{
		return wxT("Invalid unsigned integer value");
	}
};

// Factory helper for unsigned integral product types
struct wx_gui_fact_hlpr_uint
 : public wx_gui_fact_hlpr_regex<wx_gui_factory_regex_uint>
{
	unsigned long get(void)
	{
		unsigned long result;
		if(!input().GetValue().ToULong(&result))
			throw std::bad_cast();
		return result;
	}
};

template <> struct wx_gui_fact_hlpr<unsigned short int>
{
	typedef wx_gui_fact_hlpr_uint type;
};

template <> struct wx_gui_fact_hlpr<unsigned int>
{
	typedef wx_gui_fact_hlpr_uint type;
};

template <> struct wx_gui_fact_hlpr<unsigned long int>
{
	typedef wx_gui_fact_hlpr_uint type;
};

// regular expression getter for floating point types
struct wx_gui_factory_regex_float
{
	static wxString pattern(void)
	{
		return wxT("^[+-]?[[:digit:]]+\\.?[[:digit:]]*$");
	}

	static wxString message(void)
	{
		return wxT("Invalid floating point number value");
	}
};

// Factory helper for floating point product types
struct wx_gui_fact_hlpr_float
 : public wx_gui_fact_hlpr_regex<wx_gui_factory_regex_float>
{
	double get(void)
	{
		double result;
		if(!input().GetValue().ToDouble(&result))
			throw std::bad_cast();
		return result;
	}
};

template <> struct wx_gui_fact_hlpr<float>
{
	typedef wx_gui_fact_hlpr_float type;
};

template <> struct wx_gui_fact_hlpr<double>
{
	typedef wx_gui_fact_hlpr_float type;
};


// regular expression getter for character type
struct wx_gui_factory_regex_char
{
	static wxString pattern(void)
	{
		return wxT("^.$");
	}

	static wxString message(void)
	{
		return wxT("Invalid character value");
	}
};

// Factory helper for single characters
struct wx_gui_fact_hlpr_char
 : public wx_gui_fact_hlpr_regex<wx_gui_factory_regex_char>
{
	wchar_t get(void)
	{
		wxString result(input().GetValue());
		if(result.empty())
			throw std::bad_cast();
		return result.at(0);
	}
};

template <> struct wx_gui_fact_hlpr<wchar_t>
{
	typedef wx_gui_fact_hlpr_char type;
};

// Factory helper for strings
class wx_gui_fact_hlpr_string
 : public wx_gui_fact_hlpr_base<wxTextCtrl>
{
public:
	wxWindow* init_input_ctl(wxWindow* parent)
	{
		return init_input(new wxTextCtrl(parent, wxID_ANY));
	}

	std::string get(void)
	{
		return input().GetValue().utf8_str().data();
	}
};

template <> struct wx_gui_fact_hlpr<std::string>
{
	typedef wx_gui_fact_hlpr_string type;
};

// Factory helper for the tm struct using a datetime picker
class wx_gui_fact_hlpr_tm
 : public wx_gui_fact_hlpr_base<wxDatePickerCtrl> //TODO:time picker
{
public:
	wxWindow* init_input_ctl(wxWindow* parent)
	{
		return init_input(
			new wxDatePickerCtrl(
				parent,
				wxID_ANY,
				wxDefaultDateTime
			)
		);
	}

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

	std::tm get(void)
	{
		return to_tm(input().GetValue());
	}
};

template <> struct wx_gui_fact_hlpr<std::tm>
{
	typedef wx_gui_fact_hlpr_tm type;
};

// Implementation of the polymorph factory manufacturer interface
template <class Product, class SourceTraits>
class wx_gui_factory_manufacturer
 : public wx_gui_factory_utils
 , public polymorph_factory_manufacturer<Product>
{
private:
	SourceTraits source_traits;
	wx_gui_factory_data unit_data;

	typename wx_gui_fact_hlpr<Product>::type helper;
public:
	wx_gui_factory_manufacturer(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): unit_data(new wxPanel(this->unwrap_window(parent_data), wxID_ANY))
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
		unit_data.sizer->Add(
			helper.init_input_ctl(unit_data.window),
			2,
			wxEXPAND
		);
		unit_data.window->SetSizer(unit_data.sizer);
	}

	void finish(raw_ptr parent_data)
	{
		wxSizer* sizer = this->unwrap_sizer(parent_data);
		assert(sizer != nullptr);
		sizer->Add(unit_data.window, 0, wxEXPAND);
	}

	Product create(void)
	{
		return (Product)helper.get();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

