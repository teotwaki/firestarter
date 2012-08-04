/**
 * .file mirror/utils/wx_gui_factory/default.hpp
 * .brief Default implementation of the wx-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_DEFAULT_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_DEFAULT_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>
#include <mirror/utils/unused_type.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/utils.hpp>
#include <mirror/utils/wx_gui_factory/dummy_validator.hpp>
#include <mirror/utils/wx_gui_factory/enumerator.hpp>

#include <wx/wx.h>
#include <wx/textctrl.h>

MIRROR_NAMESPACE_BEGIN

template <typename Product, class SourceTraits>
class aux_input_source
{
private:
	SourceTraits source_traits;
	wxTextCtrl* info_text;

	static wxTextCtrl* make_info_text(
		const wx_input_gui_data& parent_data,
		SourceTraits& source_traits,
		unused_type construction_info
	)
	{
		wxString message(
			source_traits.get_message(
				wxT("No external sources available")
			)
		);
		wxTextCtrl* result = new wxTextCtrl(
			parent_data->window(),
			wxID_ANY,
			message,
			wxDefaultPosition,
			wxDefaultSize,
			wxTE_READONLY |
			wxTE_MULTILINE |
			wxTE_BESTWRAP,
			aux::DummyValidator(message)
		);
		result->Enable(false);
		parent_data->sizer()->Add(result, 1, wxEXPAND);
		return result;
	}

public:
	template <class ConstructionInfo>
	inline aux_input_source(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info
	): source_traits(parent_data)
	 , info_text(
		make_info_text(
			parent_data,
			source_traits,
			construction_info
		)
	){ }

	void finish(const wx_input_gui_data& parent_data)
	{
	}

	inline Product operator()(void) const
	{
		assert(!"Never should get here!");
		return *((Product*)nullptr);
	}
};

///
/**
 */
template <typename Product, class SourceTraits>
class wx_input_gui
{
private:
	// the traits
	SourceTraits source_traits;
	// the panel which will be the parent of
	// all child widgets
	wxPanel* panel;
	// the data
	typedef wx_input_gui_data data_type;
	data_type data;
	//
	// makes the data
	template <class ConstructionInfo>
	static inline data_type make_data(
		wxPanel* panel,
		const wx_input_gui_data& parent_data,
		SourceTraits& source_traits,
		ConstructionInfo construction_info,
		const Product* opt_default = (const Product*)nullptr
	)
	{
		assert(panel != nullptr);
		// enable recursive validation
		panel->SetExtraStyle(
			panel->GetExtraStyle() |
			wxWS_EX_VALIDATE_RECURSIVELY
		);
		// the sizer
		wxStaticBoxSizer* sizer = new wxStaticBoxSizer(
			wxVERTICAL,
			panel,
			source_traits.make_func_param_label(construction_info)
		);
		// set the tool tip for the panel where the input
		// widgets are going to be placed
		panel->SetToolTip(
			source_traits.make_func_param_hint(construction_info)
		);
		panel->SetSizer(sizer);
		//
		// NOTE: the panel will be added to the parent sizer
		// in the finish member function later, because
		// make_data can be called in the reverse order
		// than is the order of the constructed parameters
		// The finish function is always called in the correct
		// order
		//
		// make and return the data
		return data_type(
			parent_data,
			panel,
			sizer,
			ConstructionInfo::parameter_index::value,
			source_traits.make_func_param_data_id(construction_info)
		);
	}

	// we'll need the a factory maker
	typedef typename mirror::factory_maker<
		mirror::wx_input_gui,
		mirror::aux_input_source,
		mirror::wx_enum_gui,
		SourceTraits
	> make;

	// the factory which will be used to create instances of Product
	typename make::template factory<Product>::type fact;
public:
	template <class ConstructionInfo>
	inline wx_input_gui(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info
	): source_traits(parent_data)
	 , panel(
		new wxPanel(
			parent_data->window(),
			wxID_ANY
		)
	), data(
		make_data(
			panel,
			parent_data,
			source_traits,
			construction_info
		)
	), fact(data, construction_info)
	{ }

	void finish(const wx_input_gui_data& parent_data)
	{
		// add the panel to the parent sizer
		parent_data->sizer()->Add(
			panel,
			source_traits.default_size_proportion(),
			wxEXPAND
		);
	}

	inline Product operator()(void)
	{
		return fact();
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

