/**
 * .file mirror/utils/wx_gui_factory/dummy_validator.hpp
 * .brief Dummy validator which always returns false
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_DUMMY_VALIDATOR_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_DUMMY_VALIDATOR_1011291729_HPP

#include <wx/wx.h>
#include <wx/tipwin.h>

MIRROR_NAMESPACE_BEGIN
namespace aux {

class DummyValidator: public wxValidator
{
protected:
	wxString message;
	wxTipWindow* tip;
private:
	// no assignment
	DummyValidator& operator=(const DummyValidator&);
public:
	DummyValidator(wxString msg)
	 : message(msg)
	 , tip(nullptr)
	{ }

	DummyValidator(const DummyValidator& val)
	 : message(val.message)
	 , tip(nullptr)
	{ }

	virtual ~DummyValidator(void){ }

	// Make a clone of this validator (or return NULL) - currently necessary
	// if you're passing a reference to a validator.
	// Another possibility is to always pass a pointer to a new validator
	// (so the calling code can use a copy constructor of the relevant class).
	virtual wxObject *Clone() const
	{
		return new DummyValidator(*this);
	}

	inline wxWindow* GetControl(void)
	{
		return (wxWindow*)m_validatorWindow;
	}

	// Called when the value in the window must be validated.
	// This function can pop up an error message.
	virtual bool Validate(wxWindow *parent)
	{
		if((tip == nullptr) && !wxValidator::IsSilent())
		{
			tip = new wxTipWindow(
				GetControl(),
				message,
				100, // max length
				&tip
			);
			wxBell();
		}
		return false;
	}

	// Called to transfer data to the window
	virtual bool TransferToWindow()
	{
		return true;
	}

	// Called to transfer data from the window
	virtual bool TransferFromWindow()
	{
		return false;
	}

};


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

