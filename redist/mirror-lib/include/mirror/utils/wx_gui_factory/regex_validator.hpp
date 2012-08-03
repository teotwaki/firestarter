/**
 * .file mirror/utils/wx_gui_factory/regex_validator.hpp
 * .brief wxValidator implementation doing string regex match validation
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_REGEX_VALIDATOR_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_REGEX_VALIDATOR_1011291729_HPP

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/regex.h>
#include <wx/tipwin.h>

#include <assert.h>


MIRROR_NAMESPACE_BEGIN
namespace aux {

class RegExValidator: public wxValidator
{
protected:
	wxString pattern;
	wxRegEx regex;
	wxString message;
	wxArrayString* matches;
	wxTipWindow* tip;
	//
	bool CheckValidator(void) const
	{
		wxCHECK_MSG(
			m_validatorWindow,
			false,
			wxT("No window associated with validator")
		);
		wxCHECK_MSG(
			m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)),
			false,
			wxT("RegExValidator is only for wxTextCtrl's")
		);
		return true;
	}
private:
	// no assignment
	RegExValidator& operator=(const RegExValidator&);

	void Init(void)
	{
		this->Connect(
			wxEVT_CHAR,
			wxObjectEventFunction(&RegExValidator::OnChar)
		);
		this->Connect(
			wxEVT_COMMAND_TEXT_UPDATED,
			wxObjectEventFunction(&RegExValidator::CheckValue)
		);
	}
public:
	RegExValidator(
		wxString _pattern,
		wxString msg = wxT("Invalid value"),
		wxArrayString* strings = nullptr
	): pattern(_pattern)
	 , regex(pattern)
	 , message(msg)
	 , matches(strings)
	 , tip(nullptr)
	{
		Init();
	}

	RegExValidator(const RegExValidator& val)
	 : pattern(val.pattern)
	 , regex(pattern)
	 , message(val.message)
	 , matches(val.matches)
	 , tip(nullptr)
	{
		Init();
	}

	virtual ~RegExValidator(void){ }

	// Make a clone of this validator (or return NULL) - currently necessary
	// if you're passing a reference to a validator.
	// Another possibility is to always pass a pointer to a new validator
	// (so the calling code can use a copy constructor of the relevant class).
	virtual wxObject *Clone() const
	{
		return new RegExValidator(*this);
	}

	inline wxTextCtrl* GetControl(void)
	{
		assert(CheckValidator());
		return (wxTextCtrl *) m_validatorWindow;
	}

	inline bool QueryValue(wxString& value)
	{
		if(!CheckValidator())
			return false;
		// If window is disabled, simply return
		if (GetControl()->IsEnabled())
			value.assign(GetControl()->GetValue());
		else value.clear();
		return true;
	}

	// Called when the value in the window must be validated.
	// This function can pop up an error message.
	virtual bool Validate(wxWindow *parent)
	{
		wxString val;
		if(!QueryValue(val)) return false;
		bool ok = regex.Matches(val);
		if(!ok && (tip == nullptr) && !wxValidator::IsSilent())
		{
			tip = new wxTipWindow(
				GetControl(),
				message,
				100, // max length
				&tip
			);
			wxBell();
		}
		return ok;
	}

	// Called to transfer data to the window
	virtual bool TransferToWindow()
	{
		return true;
	}

	// Called to transfer data from the window
	virtual bool TransferFromWindow()
	{
		wxString val;
		if(!QueryValue(val)) return false;
		if(regex.Matches(val))
		{
			if(matches != nullptr)
			{
				matches->clear();
				for(int i=0,n=regex.GetMatchCount();i<n;++i)
				{
					size_t start, len;
					if(!regex.GetMatch(&start, &len, i))
					{
						return false;
					}
					matches->Add(wxString(val, start, len));
				}
			}
			return true;
		}
		return false;
	}

	void OnChar(wxKeyEvent& event)
	{
		// schedule the mini-validation to occur after
		// this event is processed and the value is
		// updated
		wxCommandEvent new_event(wxEVT_COMMAND_TEXT_UPDATED);
		AddPendingEvent(new_event);
		// accept the event
		event.Skip();
	}

	void CheckValue(wxCommandEvent& event)
	{
		bool accept = false;
		wxString val;
		if(QueryValue(val))
		{
			// if the new string matches the expression
			if(regex.Matches(val))
			{
				// accept the event
				accept = true;
			}
		}
		// if the value is acceptable ..
		if(accept)
		{
			GetControl()->SetBackgroundColour(
				wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW)
			);
		}
		else
		{
			GetControl()->SetBackgroundColour(
				wxColour(0xFF, 0xA0, 0xA0)
			);
			if(!wxValidator::IsSilent())
			{
				wxBell();
			}
		}
		// accept the event
		event.Skip();
	}
};


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

