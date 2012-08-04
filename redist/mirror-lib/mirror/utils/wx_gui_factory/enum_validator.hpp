/**
 * .file mirror/utils/wx_gui_factory/enum_validator.hpp
 * .brief Enumeration validator which returns false if no value selected
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_ENUM_VALIDATOR_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_ENUM_VALIDATOR_1011291729_HPP

#include <wx/wx.h>
#include <wx/tipwin.h>

MIRROR_NAMESPACE_BEGIN
namespace aux {

class EnumValidator: public wxValidator
{
protected:
	wxString message;
	wxTipWindow* tip;
	const int enum_count;
private:
	// no assignment
	EnumValidator& operator=(const EnumValidator&);
public:
	EnumValidator(wxString msg, int size)
	 : message(msg)
	 , tip(nullptr)
	 , enum_count(size)
	{ }

	EnumValidator(const EnumValidator& val)
	 : message(val.message)
	 , tip(nullptr)
	 , enum_count(val.enum_count)
	{ }

	virtual ~EnumValidator(void){ }

	// Make a clone of this validator (or return NULL) - currently necessary
	// if you're passing a reference to a validator.
	// Another possibility is to always pass a pointer to a new validator
	// (so the calling code can use a copy constructor of the relevant class).
	virtual wxObject *Clone() const
	{
		return new EnumValidator(*this);
	}

	inline wxWindow* GetControl(void)
	{
		return (wxWindow*)m_validatorWindow;
	}

	wxComboBox* enumerator(void)
	{
		wxComboBox* result = static_cast<wxComboBox*>(GetWindow());
		assert(result != nullptr);
		return result;
	}

	bool input_valid(void)
	{
		int sel = enumerator()->GetCurrentSelection();
		return (sel >= 0) && (sel < enum_count);
	}

	// Called when the value in the window must be validated.
	// This function can pop up an error message.
	virtual bool Validate(wxWindow *parent)
	{
		if(input_valid()) return true;
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
		return input_valid();
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

