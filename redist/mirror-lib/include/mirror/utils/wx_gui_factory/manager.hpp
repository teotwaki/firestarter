/**
 * .file mirror/utils/wx_gui_factory/manager.hpp
 * .brief Manager component for the wx-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_MANAGER_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_MANAGER_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/wx_gui_factory/default.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/mirror_fwd.hpp>
#include <mirror/utils/unused_type.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/bookctrl.h>

#include <list>

MIRROR_NAMESPACE_BEGIN

/// Specializaion of the wx_input_gui factory for the manager component
template <class SourceTraits>
class wx_input_gui<void, SourceTraits>
{
private:
	typedef typename SourceTraits::base_book_ctrl BaseBookCtrl;

	// The control for keeping track of which constructor is to be used
	class BookCtrl : public BaseBookCtrl
	{
	private:
		wxWindow* CurrentPage(void) const
		{
			return this->GetPage(this->GetSelection());
		}

		typename wx_input_gui_data::weak_ref data_ref;
	public:
		BookCtrl(wxWindow* parent, int id)
		 : BaseBookCtrl(parent, id)
		{ }

		// Post construction initialization which must be done
		// *after* all the pages are added
		void initialize(const wx_input_gui_data& data)
		{
			// use the data of the manager
			data_ref = data;
			// enable the event handlers
			this->Connect(
				wxEVT_KEY_DOWN,
				wxKeyEventHandler(BookCtrl::HandleKeyPress)
			);
			this->Connect(
				wxEVT_KEY_UP,
				wxKeyEventHandler(BookCtrl::IgnoreKeyPress)
			);
			this->Connect(
				wxEVT_NAVIGATION_KEY,
				wxNavigationKeyEventHandler(
					BookCtrl::HandleNavigation
				)
			);
			// and select the first constructor
			this->SetSelection(0);

		}

		virtual bool Validate(void)
		{
			wxWindow* visible_child(CurrentPage());
			if(visible_child)
				return visible_child->Validate();
			else return true;
		}

		virtual bool TransferDataFromWindow(void)
		{
			wxWindow* visible_child(CurrentPage());
			if(visible_child)
				return visible_child->TransferDataFromWindow();
			else return true;
		}

		void HandleNavigation(wxNavigationKeyEvent& evt)
		{
			wx_input_gui_data book_data(data_ref);
			// navigating forward
			if(evt.GetDirection())
				book_data->focus_next();
			// navigating backward
			else book_data->focus_prev();
			evt.StopPropagation();
		}

		void IgnoreKeyPress(wxKeyEvent& evt)
		{
			switch(evt.GetKeyCode())
			{
				case WXK_TAB:
				case WXK_PAGEUP:
				case WXK_UP:
				case WXK_LEFT:
				case WXK_PAGEDOWN:
				case WXK_DOWN:
				case WXK_RIGHT:
				case WXK_HOME:
				case WXK_END:
					break;
				default: evt.Skip();
			}
		}

		void HandleKeyPress(wxKeyEvent& evt)
		{
			// we need some pages to work with
			assert(this->GetPageCount() > 0);
			// the navigation offset
			int ofs = 1;
			//
			// do something based on the key that was pressed
			switch(evt.GetKeyCode())
			{
			case WXK_TAB:
			{
				wxNavigationKeyEvent nav_evt;
				nav_evt.SetFromTab(true);
				nav_evt.SetDirection(!evt.ShiftDown());
				nav_evt.SetCurrentFocus(this);
				this->ProcessEvent(nav_evt);
				break;
			}
			case WXK_PAGEUP:
				ofs = 4;
			case WXK_UP:
			case WXK_LEFT:
			{
				int sel = this->GetSelection();
				if(ofs > sel) ofs = sel;
				if(ofs > 0) this->SetSelection(sel - ofs);
				break;
			}
			case WXK_PAGEDOWN:
				ofs = 4;
			case WXK_DOWN:
			case WXK_RIGHT:
			{
				int sel = this->GetSelection();
				if(ofs > int(this->GetPageCount() - sel - 1))
					ofs = this->GetPageCount() - sel - 1;
				if(ofs > 0) this->SetSelection(sel + ofs);
				break;
			}
			case WXK_HOME:
			{
				this->SetSelection(0);
				break;
			}
			case WXK_END:
			{
				this->SetSelection(this->GetPageCount() - 1);
				break;
			}
			default: evt.Skip();
			}
		}
	};

	// the book ctrl
	SourceTraits source_traits;
	BookCtrl* book_ctl;
	wx_input_gui_data data;
	//
	// makes the component tracking which constructor should be invoked
	static BookCtrl* make_book_ctl(
		const wx_input_gui_data& parent_data
	)
	{
		// make the book ctrl
		BookCtrl* book_ctl = new BookCtrl(
			parent_data->window(),
			wxID_ANY
		);
		book_ctl->SetExtraStyle(
			book_ctl->GetExtraStyle() |
			wxWS_EX_VALIDATE_RECURSIVELY
		);

		// add the widget to the sizer
		parent_data->sizer()->Add(book_ctl, 1, wxEXPAND);
		// return the pointer to the newly created book control
		return book_ctl;
	}
	// list of parameters for the individual pages
	// WARNING: the list may be in reverse order compared
	// to the order of the parameter declaration
	::std::list<wx_input_gui_data> params;

	template <class EnumInfo>
	wx_input_gui_data append_constructor(
		const wxString& constructor_label,
		const wxString& constructor_data_id,
		const EnumInfo& enumeration_info,
		int constructor_index
	)
	{
		// create a panel and a sizer for the page
		wxPanel* constructor_panel = new wxPanel(
			book_ctl,
			wxID_ANY
		);
		// enable recursive validation
		constructor_panel->SetExtraStyle(
			constructor_panel->GetExtraStyle() |
			wxWS_EX_VALIDATE_RECURSIVELY
		);
		// create the the sizer for the constructor parameter
		// input widgets
		wxBoxSizer* ctr_param_sizer = new wxBoxSizer(wxVERTICAL);
		constructor_panel->SetSizer(ctr_param_sizer);
		// if we are enumerating the constructors backward
		if(enumeration_info.going_backward())
		{
			// prepend a new page
			book_ctl->InsertPage(
				0,
				constructor_panel,
				constructor_label
			);
		}
		// if we are not enumerating the constructors backward
		else
		{
			// add a new page
			book_ctl->AddPage(
				constructor_panel,
				constructor_label
			);
		}
		// make and remember the input data
		// for this constructor
		params.push_back(
			wx_input_gui_data(
				data,
				constructor_panel,
				ctr_param_sizer,
				constructor_index,
				constructor_data_id
			)
		);
		// return the enlisted data
		return params.back();
	}

public:
	inline wx_input_gui(
		const wx_input_gui_data& parent_data,
		unused_type context
	): source_traits(parent_data)
	 , book_ctl(make_book_ctl(parent_data))
	 , data(parent_data, book_ctl, SourceTraits::make_manager_data_id())
	{ }

	void finish(const wx_input_gui_data& parent_data)
	{
		book_ctl->initialize(data);
	}

	/// Prepares the widgets before creating the inputs for a constructor
	/** This function is called by the factory generator
	 */
	template <class ConstructorInfo>
	inline wx_input_gui_data add_constructor(
		const wx_input_gui_data& parent_data,
		ConstructorInfo constructor_info
	)
	{
		typename ConstructorInfo::product meta_product;
		typename ConstructorInfo::constructor_index constructor_index;
		return append_constructor(
			source_traits.make_constructor_label(
				meta_product,
				constructor_index
			),
			SourceTraits::make_constructor_data_id(
				meta_product,
				constructor_index
			),
			constructor_info.enumeration_info,
			ConstructorInfo::constructor_index::value
		);
	}

	inline int index(void)
	{
		return book_ctl->GetSelection();
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

