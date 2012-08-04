/**
 * @file lagoon/utils/wx_gui_factory/suppliers.hpp
 * @brief wxWidgets-based GUI polymorphic factory suppliers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_SUPPLIERS_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_SUPPLIERS_1011291729_HPP

#include <cassert>
#include <mirror/utils/wx_gui_factory/dummy_validator.hpp>

LAGOON_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_gui_factory_suppliers
 : public wx_gui_factory_utils
 , public polymorph_factory_suppliers
{
private:
	SourceTraits source_traits;

	wxTextCtrl* info_text;

	wxTextCtrl* make_info_text(
		raw_ptr parent_data,
		const shared<meta_parameter>& param
	) const
	{
		wxString message(
			source_traits.get_message(
				wxT("No external sources available")
			)
		);
		wxTextCtrl* result = new wxTextCtrl(
			this->unwrap_window(parent_data),
			wxID_ANY,
			message,
			wxDefaultPosition,
			wxDefaultSize,
			wxTE_READONLY |
			wxTE_MULTILINE |
			wxTE_BESTWRAP,
			mirror::aux::DummyValidator(message)
		);
		result->Enable(false);
		this->unwrap_sizer(parent_data)->Add(result, 1, wxEXPAND);
		return result;
	}
public:
	wx_gui_factory_suppliers(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): info_text(make_info_text(parent_data, param))
	{ }

	raw_ptr get(void)
	{
		assert(!"No external sources available");
		return raw_ptr();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

