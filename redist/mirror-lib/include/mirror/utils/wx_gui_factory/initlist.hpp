/**
 * .file mirror/utils/wx_gui_factory/initlist.hpp
 * .brief Implementation of the wx-based factory template for containers
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_INITLIST_1102091707_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_INITLIST_1102091707_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>
#include <mirror/utils/unused_type.hpp>
#include <mirror/utils/wx_gui_factory/string.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/utils.hpp>
#include <mirror/utils/wx_gui_factory/dummy_validator.hpp>
#include <mirror/utils/wx_gui_factory/enumerator.hpp>
#include <mirror/stream/simple_struct.hpp>

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/button.h>

#include <initializer_list>
#include <sstream>
#include <list>

MIRROR_NAMESPACE_BEGIN

template <typename Element, class SourceTraits>
class wx_input_gui<std::initializer_list<Element>, SourceTraits>
{
private:
	// the traits
	SourceTraits source_traits;

	// the panel which will be the parent of
	// all child widgets
	class item_panel : public wxPanel
	{
	public:
		typedef std::list<Element> element_list;
		element_list elements;
	private:
		wxComboBox* item_combo;
		wxStaticBoxSizer* main_sizer;
		wxButton* erase_button;
		wxButton* append_button;

		aux::wx_gui_factory_dialog_intf<Element>* dialog;
		//
		// erase button click handler
		void erase_item(wxCommandEvent& event)
		{
			assert(item_combo != nullptr);
			int index = item_combo->GetSelection();
			if(index != wxNOT_FOUND)
			{
				item_combo->Delete(index);
				auto pos = elements.begin();
				while(index > 0)
				{
					assert(pos != elements.end());
					++pos;
					--index;
				}
				elements.erase(pos);
				item_combo->SetValue(wxEmptyString);
			}
		}

		wxString make_item_title(const Element& value)
		{
			std::stringstream tmp;
			tmp << stream::to_simple_struct::from(value);
			return wx_input_gui_str(tmp.str());
		}
		//
		// append button click handler
		void append_item(wxCommandEvent& event)
		{
			assert(dialog != nullptr);
			try
			{
				elements.push_back(dialog->create());
				item_combo->Append(
					make_item_title(elements.back())
				);
			}
			catch(...){ }
		}
	public:
		template <class ConstructionInfo>
		item_panel(
			wxWindow* parent,
			SourceTraits& source_traits,
			ConstructionInfo construction_info
		): wxPanel(parent, wxID_ANY)
		{
			// enable recursive validation
			this->SetExtraStyle(
				this->GetExtraStyle() |
				wxWS_EX_VALIDATE_RECURSIVELY
			);
			// the sizer
			main_sizer = new wxStaticBoxSizer(
				wxHORIZONTAL,
				this,
				source_traits.make_func_param_label(
					construction_info
				)
			);
			// add the item combobox
			item_combo = new wxComboBox(
				this,
				wxID_ANY,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				wxArrayString(),
				wxCB_READONLY
			);
			main_sizer->Add(item_combo, 1, wxEXPAND);
			// add the erase button
			erase_button = new wxButton(
				this,
				__LINE__,
				source_traits.get_erase_label()
			);
			Connect(
				erase_button->GetId(),
				wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(item_panel::erase_item)
			);
			main_sizer->Add(erase_button, 0, wxEXPAND);
			// add the append button
			append_button = new wxButton(
				this,
				__LINE__,
				source_traits.get_append_label()
			);
			Connect(
				append_button->GetId(),
				wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(item_panel::append_item)
			);
			main_sizer->Add(append_button, 0, wxEXPAND);
			// set the tool tip for the panel where the input
			// widgets are going to be placed
			this->SetToolTip(
				source_traits.make_func_param_hint(
					construction_info
				)
			);
			this->SetSizer(main_sizer);
			dialog = aux::make_wx_gui_factory_dialog<
				Element,
				SourceTraits
			>(this, source_traits.get_append_label());
		}

		wxSizer* sizer(void)
		{
			return main_sizer;
		}
	}* panel;
	// the data
	typedef wx_input_gui_data data_type;
	data_type data;
	//
	// makes the data
	template <class ConstructionInfo>
	inline data_type make_data(
		const wx_input_gui_data& parent_data,
		SourceTraits& source_traits,
		ConstructionInfo construction_info
	)
	{
		assert(panel != nullptr);
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
			panel->sizer(),
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
public:
	template <class ConstructionInfo>
	inline wx_input_gui(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info
	): source_traits(parent_data)
	 , panel(
		new item_panel(
			parent_data->window(),
			source_traits,
			construction_info
		)
	), data(
		make_data(
			parent_data,
			source_traits,
			construction_info
		)
	)
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

	inline typename item_panel::element_list operator()(void)
	{
		assert(panel != nullptr);
		return panel->elements;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

