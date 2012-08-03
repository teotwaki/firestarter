/**
 * .file mirror/utils/wx_gui_factory/enumerator.hpp
 * .brief Default implementation of the wx-based enumerator
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_ENUMERATOR_1101171618_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_ENUMERATOR_1101171618_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>
#include <mirror/utils/unused_type.hpp>
#include <mirror/utils/wx_gui_factory/data.hpp>
#include <mirror/utils/wx_gui_factory/utils.hpp>
#include <mirror/utils/wx_gui_factory/enum_validator.hpp>

#include <wx/wx.h>
#include <wx/combobox.h>

MIRROR_NAMESPACE_BEGIN

template <typename Product, class SourceTraits>
class wx_enum_gui
 : public wx_input_gui_utils<SourceTraits>
{
private:
	SourceTraits source_traits;
	wxComboBox* combo_ctl;
	wx_input_gui_data data;

	struct combo_maker
	{
		SourceTraits& source_traits;

		combo_maker(SourceTraits& traits)
		 : source_traits(traits)
		{ }

		inline wxComboBox* operator()(wxWindow* parent) const
		{
			typedef MIRRORED_ENUM(Product) MetaEnum;
			wxArrayString item_names;
			for(int i=0; i!= MetaEnum::size::value; ++i)
			{
				item_names.Add(
					source_traits.get_enum_value_label(
						MetaEnum(),
						i
					)
				);
			}
			wxString message(
				source_traits.get_message(
					wxT("A value must be selected")
				)
			);
			wxComboBox* result = new wxComboBox(
				parent,
				wxID_ANY,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				item_names,
				wxCB_READONLY,
				aux::EnumValidator(
					message,
					MetaEnum::size::value
				)
			);
			return result;
		}
	};

public:
	template <class ConstructionInfo>
	inline wx_enum_gui(
		const wx_input_gui_data& parent_data,
		ConstructionInfo construction_info
	): wx_input_gui_utils<SourceTraits>(parent_data)
	 , source_traits(parent_data)
	 , combo_ctl(
		this->make_input_unit(
			combo_maker(source_traits),
			parent_data,
			construction_info
		)
	), data(
		parent_data,
		combo_ctl,
		ConstructionInfo::parameter_index::value,
		SourceTraits::make_func_param_data_id(construction_info)
	){ }

	void finish(const wx_input_gui_data& parent_data)
	{
		this->finish_input_unit(combo_ctl, parent_data, data);
	}

	inline Product operator()(void) const
	{
		return MIRRORED_ENUM(Product)::value(
			combo_ctl->GetCurrentSelection()
		);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

