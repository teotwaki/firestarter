/**
 * @file lagoon/utils/wx_gui_factory/manager.hpp
 * @brief wxWidgets-based GUI polymorphic factory manager
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_MANAGER_1011291729_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_MANAGER_1011291729_HPP

#include <list>

LAGOON_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_gui_factory_manager
 : public wx_gui_factory_utils
 , public polymorph_factory_manager
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::base_book_ctrl base_book_ctrl;

	class book_ctrl : public base_book_ctrl
	{
	private:
		wxWindow* CurrentPage(void) const
		{
			return this->GetPage(this->GetSelection());
		}
	public:
		book_ctrl(wx_gui_factory_data* parent_data)
		 : base_book_ctrl(parent_data->window, wxID_ANY)
		{
			this->SetExtraStyle(
				this->GetExtraStyle() |
				wxWS_EX_VALIDATE_RECURSIVELY
			);
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
	};

	book_ctrl* book_ctl;

	std::list<wx_gui_factory_data> params;

public:
	wx_gui_factory_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	): book_ctl(new book_ctrl(this->unwrap(parent_data)))
	{
	}

	void finish(raw_ptr parent_data)
	{
		wxSizer* sizer = this->unwrap_sizer(parent_data);
		assert(sizer != nullptr);
		sizer->Add(book_ctl, 1, wxEXPAND);
		book_ctl->SetSelection(0);
	}

	raw_ptr add_constructor(
		raw_ptr parent_data,
		const shared<meta_constructor>& constructor,
		const polymorph_factory_context& context,
		bool backward_iteration
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
		//
		wxString constructor_label(
			source_traits.make_constructor_label(constructor)
		);
		// if we are enumerating the constructors backward
		if(backward_iteration)
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
		// make and remember the input data for this constructor
		params.push_back(
			wx_gui_factory_data(
				constructor_panel,
				ctr_param_sizer
			)
		);
		return raw_ptr(&params.back());
	}

	int index(void)
	{
		return book_ctl->GetSelection();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

