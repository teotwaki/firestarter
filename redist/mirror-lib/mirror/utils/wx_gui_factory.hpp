/**
 * @file mirror/utils/wx_gui_factory.hpp
 * @brief wxWidgets-based GUI factory template for the factory generator
 * utility
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_1011291729_HPP

#include <mirror/config.hpp>

// wx gui factory-related
#include <mirror/utils/wx_gui_factory/dialog_interface.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/default.hpp>
#include <mirror/utils/wx_gui_factory/basic.hpp>
#include <mirror/utils/wx_gui_factory/boolean.hpp>
#include <mirror/utils/wx_gui_factory/manager.hpp>
#include <mirror/utils/wx_gui_factory/enumerator.hpp>
#include <mirror/utils/wx_gui_factory/initlist.hpp>

#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/statline.h>

#include <stdexcept>

MIRROR_NAMESPACE_BEGIN
namespace aux {

//  Base template class for other factory dialogs
template <class SourceTraits>
class wx_gui_factory_dialog_base
 : public wxDialog
{
protected:
	// the ok button
	wxButton* ok_button;
	//
	// the input widgets data
	typedef wx_input_gui_data data_type;
	data_type input_data;
	// The input source traits
	SourceTraits source_traits;
	// data for the dialog buttons
	data_type button_data;
private:

	/*  prepares the dialog and creates the necessary widgets
	 *  before creating the input widgets by the factory template
	 *  and the factory generator utility
	 */
	data_type make_input_data(void)
	{
		//
		// get the pointer to the dialog
		wxDialog* dialog = this;
		//
		// make the main panel of the dialog where all
		// the input controls are going to be
		wxPanel* main_panel = new wxPanel(dialog, wxID_ANY);
		main_panel->SetExtraStyle(
			main_panel->GetExtraStyle() |
			wxWS_EX_VALIDATE_RECURSIVELY
		);
		// create the sizer for the input controls
		wxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
		main_panel->SetSizer(main_sizer);
		//
		// create the dialog's main sizer
		wxSizer* dialog_sizer = new wxBoxSizer(wxVERTICAL);
		// dialog top padding
		dialog_sizer->AddSpacer(SourceTraits::padding_top());
		// the main panel where the input widgets
		// will be created and placed
		dialog_sizer->Add(main_panel, 1, wxEXPAND);
		// a horizontal line with minor spacers
		dialog_sizer->AddSpacer(SourceTraits::minor_spacer_width());
		dialog_sizer->Add(
			new wxStaticLine(
				main_panel,
				wxID_ANY,
				wxDefaultPosition,
				wxDefaultSize,
				wxLI_HORIZONTAL
			)
		);
		dialog_sizer->AddSpacer(SourceTraits::minor_spacer_width());
		// standard button set
		wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
		button_sizer->AddStretchSpacer();
		button_sizer->Add(new wxButton(dialog, wxID_OPEN));
		button_sizer->Add(new wxButton(dialog, wxID_SAVE));
		button_sizer->AddSpacer(SourceTraits::major_spacer_width());
		button_sizer->Add(new wxButton(dialog, wxID_CLEAR));
		button_sizer->AddSpacer(SourceTraits::major_spacer_width());
		ok_button = new wxButton(dialog, wxID_OK);
		button_sizer->Add(ok_button);
		button_sizer->AddSpacer(SourceTraits::minor_spacer_width());
		button_sizer->Add(new wxButton(dialog, wxID_CANCEL));
		// add the button sizer
		dialog_sizer->Add(button_sizer, 0, wxEXPAND);
		// bottom padding
		dialog_sizer->AddSpacer(SourceTraits::padding_bottom());
		//
		// create the dialog's outer sizer
		wxSizer* dialog_outer_sizer = new wxBoxSizer(wxHORIZONTAL);
		// left padding
		dialog_outer_sizer->AddSpacer(SourceTraits::padding_left());
		// the dialog vertical sizer
		dialog_outer_sizer->Add(dialog_sizer, 1, wxEXPAND);
		// right padding
		dialog_outer_sizer->AddSpacer(SourceTraits::padding_right());
		// assign the sizer
		dialog->SetSizer(dialog_outer_sizer);
		//
		// return the data
		return data_type(
			main_panel,
			main_sizer,
			SourceTraits::make_root_data_id()
		);
	}

	void clear_input(wxCommandEvent& evt)
	{
		input_data->clear_input();
	}

	bool pick_state_file_path(wxString& path, bool saving)
	{
		wxFileDialog file_dialog(
			this,
			wxGetTranslation(wxT("Choose a file")),
			wxEmptyString, // default dir
			wxEmptyString, // default filename
			wxT("*.xml"),  // default wildcard
			saving ? wxFD_SAVE : wxFD_OPEN
		);
		if(file_dialog.ShowModal() == wxOK)
		{
			path = file_dialog.GetPath();
		}
		return false;
	}

	void load_state(wxCommandEvent& evt)
	{
		wxString file_path;
		if(pick_state_file_path(file_path, false))
		{
			wxXmlDocument xmldoc(file_path);
			// TODO
			//input_data->load_state(xmldoc);
		}
	}

	void save_state(wxCommandEvent& evt)
	{
		wxString file_path;
		if(pick_state_file_path(file_path, true))
		{
			wxXmlDocument xmldoc;
			input_data->save_state(xmldoc);
			xmldoc.Save(file_path);
		}
	}
protected:
	// creates the dialog and prepares the basic widgets
	wx_gui_factory_dialog_base(
		wxWindow* parent_window,
		const wxString& title
	): wxDialog(
		parent_window,
		wxID_ANY,
		title,
		wxDefaultPosition,
		wxDefaultSize,
		wxDEFAULT_DIALOG_STYLE |
		wxRESIZE_BORDER
	), ok_button(nullptr)
	 , input_data(make_input_data())
	 , source_traits(input_data)
	{
		Connect(
			wxID_SAVE,
			wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(
				wx_gui_factory_dialog_base::save_state
			)
		);
		Connect(
			wxID_OPEN,
			wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(
				wx_gui_factory_dialog_base::load_state
			)
		);
		Connect(
			wxID_CLEAR,
			wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(
				wx_gui_factory_dialog_base::clear_input
			)
		);
	}

	//  post construction initialization
	/*  Called by the derived classes after the construction
 	 *  This function takes care of things like the final
	 *  size recalculation and enables recursive validation
	 *  for the input widgets
	 */
	void post_construct_init(void)
	{
		GetSizer()->SetSizeHints(this);
		SetMinSize(GetSize());
		SetExtraStyle(
			GetExtraStyle() |
			wxWS_EX_VALIDATE_RECURSIVELY
		);
		button_data = data_type(
			input_data,
			ok_button,
			input_data->child_count(),
			SourceTraits::make_helper_data_id()
		);
		input_data->init_navigation();
	}

	//  Shows the dialog and returns true if all input data is OK
	/*  Note this works properly only if the validators for
	 *  the input widgets are set-up properly
	 */
	bool get_input(void)
	{
		Center();
		return (ShowModal() == wxID_OK);
	}
};

} // namespace aux

/// A wxWidgets-based dialog for construction of the @a Product
/** This template class can be used to create wxWidgets-based
 *  dialogs which have the necessary input widgets for getting
 *  all parameter values necessary for the construction of
 *  instances of the Product type.
 *
 *  The dialog can be configured by the means of the @a SourceTraits
 *  template parameter.
 */
template <class Product, class SourceTraits>
class wx_gui_factory_dialog
 : public aux::wx_gui_factory_dialog_base<SourceTraits>
 , public aux::wx_gui_factory_dialog_intf<Product>
{
private:
	static wxString make_title(wxString title)
	{
		if(title.empty())
		{
			// TODO: make up some title if empty
		}
		return title;
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
	wx_gui_factory_dialog(
		const wxString& dialog_title,
		wxWindow* parent_window = (wxWindow*)nullptr
	): aux::wx_gui_factory_dialog_base<SourceTraits>(
		parent_window,
		make_title(dialog_title)
	), fact(this->input_data)
	{
		// initialize the base dialog
		this->post_construct_init();
	}

	/// Show the dialog, get the data and create an instance of Product
	Product* new_(void)
	{
		// if we've got some input
		if(this->get_input())
		{
			// use the factory to create
			// an instance of Product
			return fact.new_();
		}
		return nullptr;
	}

	Product create(void)
	{
		// if we've aint't got any input
		if(!this->get_input())
			throw std::runtime_error("Aborted by user");
		// use the factory to create
		// an instance of Product
		return fact();
	}
};

namespace aux {

template <typename Product, class SourceTraits>
inline wx_gui_factory_dialog_intf<Product>* make_wx_gui_factory_dialog(
	wxWindow* parent,
	const wxString& caption
)
{
	return new mirror::wx_gui_factory_dialog<Product, SourceTraits>(
		caption,
		parent
	);
}

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

