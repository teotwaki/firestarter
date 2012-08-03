/**
 * .file mirror/utils/wx_gui_factory/basic.hpp
 * .brief Implementation of the wx-gui factory template for basic native types
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_BASIC_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_BASIC_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/wx_gui_factory/string.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/utils.hpp>
#include <mirror/utils/wx_gui_factory/regex_validator.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>

MIRROR_NAMESPACE_BEGIN

template <class Product, class SourceTraits>
class wx_input_gui_basic
 : public wx_input_gui_utils<SourceTraits>
{
private:
	wxTextCtrl* text_ctl;
	wx_input_gui_data data;

	struct text_ctl_maker
	{
		wxString pattern;
		wxString message;

		text_ctl_maker(
			const wxString& pat,
			const wxString& msg
		): pattern(pat)
		 , message(msg)
		{ }

		inline wxTextCtrl* operator()(wxWindow* parent) const
		{
			if(pattern.empty())
				return new wxTextCtrl(
					parent,
					wxID_ANY
				);
			else return new wxTextCtrl(
				parent,
				wxID_ANY,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				0,
				aux::RegExValidator(pattern, message)
			);
		}
		typedef wxTextCtrl* result_type;
	};

protected:
	wxString get_text(void) const
	{
		return text_ctl->GetValue();
	}

	void init(const std::string* opt_def)
	{
		if(opt_def) text_ctl->SetValue(wx_input_gui_str(*opt_def));
	}

	template <typename Default>
	void init(const Default* opt_def)
	{
		// TODO: this needs to be fixed for integral
		// and floating point types
		//if(opt_def) text_ctl->SetValue(*opt_def);
	}

	template <typename Default>
	void init(const wxString& fmt, const Default* opt_def)
	{
		if(opt_def) text_ctl->SetValue(
			wxString::Format(
				fmt,
				*opt_def
			)
		);
	}
public:
	template <class ConstructionInfo>
	wx_input_gui_basic(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info,
		Product const* opt_default,
		const wxString& pattern_sid,
		const wxString& pattern,
		const wxString& message
	): wx_input_gui_utils<SourceTraits>(parent_data)
	 , text_ctl(
		// use the utilities and the text widget maker
		// to make the input unit with the input widget
		this->make_input_unit(
			text_ctl_maker(
				this->source_traits.get_value_regex_pattern(
					pattern_sid,
					pattern,
					construction_info
				),
				this->source_traits.get_invalid_value_message(
					message,
					construction_info
				)
			),
			parent_data,
			construction_info
		)
	), data(
		parent_data,
		text_ctl,
		ConstructionInfo::parameter_index::value,
		SourceTraits::make_func_param_data_id(construction_info)
	)
	{ }

	void finish(const wx_input_gui_data& parent_data)
	{
		this->finish_input_unit(text_ctl, parent_data, data);
	}
};

#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI( \
	INPUT_SOURCE, \
	PRODUCT, \
	CONSTRUCTOR_BODY, \
	FUNCTOR_BODY, \
	PATTERN_SID, \
	PATTERN, \
	MESSAGE \
) \
template <class SourceTraits> \
class INPUT_SOURCE< PRODUCT, SourceTraits> \
 : public wx_input_gui_basic< PRODUCT, SourceTraits> \
{ \
private: \
	typedef wx_input_gui_basic< PRODUCT, SourceTraits> \
		base_class; \
public: \
	template < class ConstructionInfo > \
	inline INPUT_SOURCE( \
		const wx_input_gui_data& parent_data, \
		ConstructionInfo construction_info, \
		PRODUCT const* opt_default = (PRODUCT const*)nullptr \
	): base_class( \
		parent_data, \
		construction_info, \
		opt_default, \
		wxT( "$(" PATTERN_SID ")" ), \
		wxT( PATTERN ), \
		wxT( MESSAGE ) \
	) CONSTRUCTOR_BODY \
 \
	inline PRODUCT operator()(void) FUNCTOR_BODY \
};

// string type
#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_STRING( \
	TYPE, \
	INPUT_SOURCE, \
	CONV_PROC \
) MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI( \
	INPUT_SOURCE, \
	TYPE, \
	{this->init(opt_default);}, \
	{return TYPE(this->get_text().CONV_PROC(wxConvUTF8));}, \
	"", \
	"", \
	"" \
)

// character type
#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_CHAR( \
	TYPE, \
	INPUT_SOURCE \
) MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI( \
	INPUT_SOURCE, \
	TYPE, \
	{this->init(opt_default);}, \
	{return TYPE(this->get_text().at(0));}, \
	"__"  #TYPE "_regex", \
	"^.$", \
	"Invalid character value" \
)

#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_WX_CONVERT( \
	INPUT_SOURCE, \
	TYPE, \
	TEMP_TYPE, \
	FORMAT_STRING, \
	CONVERSION, \
	PATTERN_SID, \
	PATTERN, \
	MESSAGE \
) MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI( \
	INPUT_SOURCE, \
	TYPE, \
	{this->init(wxT(FORMAT_STRING), opt_default);}, \
	{ \
		TEMP_TYPE result; \
		if(!this->get_text().CONVERSION(&result)) \
			throw ::std::bad_cast(); \
		return (TYPE)(result); \
	}, \
	PATTERN_SID, \
	PATTERN, \
	MESSAGE \
)

#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_UINT( \
	TYPE, \
	INPUT_SOURCE \
) MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_WX_CONVERT( \
	INPUT_SOURCE, \
	TYPE, \
	unsigned long, \
	"%ul", \
	ToULong, \
	"__ulongint_regex", \
	"^[+]?[[:digit:]]+$", \
	"Invalid unsigned integer value" \
)

#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_INT( \
	TYPE, \
	INPUT_SOURCE \
) MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_WX_CONVERT( \
	INPUT_SOURCE, \
	TYPE, \
	long, \
	"%dl", \
	ToLong, \
	"__longint_regex", \
	"^[+-]?[[:digit:]]+$", \
	"Invalid integer value" \
)

#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_FLOAT( \
	TYPE, \
	INPUT_SOURCE \
) MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_WX_CONVERT( \
	INPUT_SOURCE, \
	TYPE, \
	double, \
	"%f", \
	ToDouble, \
	"__float_regex", \
	"^[+-]?[[:digit:]]+\\.?[[:digit:]]*$", \
	"Invalid floating point number value" \
)

#define MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_NATIVE(INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_UINT(unsigned short, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_UINT(unsigned int, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_UINT(unsigned long, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_INT(short, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_INT(int, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_INT(long, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_FLOAT(float, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_FLOAT(double, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_STRING(std::string, INPUT_SOURCE, mb_str) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_STRING(std::wstring, INPUT_SOURCE, wc_str) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_CHAR(char, INPUT_SOURCE) \
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_CHAR(wchar_t, INPUT_SOURCE)

#ifndef MIRROR_WX_GUI_FACTORY_ELABORATED_NATIVE_INPUT
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_NATIVE(wx_input_gui)
#endif
MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_NATIVE(aux_input_source)

#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_NATIVE
#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_STRING
#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_CHAR
#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_UINT
#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_INT
#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_FLOAT
#undef MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_INPUT_GUI_WX_CONVERT

// implementation of the state_saver for the TextCtrl
template <>
struct wx_input_gui_state_saver<wxTextCtrl>
{
	static bool save_state(wxTextCtrl* widget, wxString& dst)
	{
		assert(widget != nullptr);
		dst.assign(widget->GetValue());
		return true;
	}

	static bool load_state(wxTextCtrl* widget, const wxString& src)
	{
		assert(widget != nullptr);
		widget->SetValue(src);
		return true;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

