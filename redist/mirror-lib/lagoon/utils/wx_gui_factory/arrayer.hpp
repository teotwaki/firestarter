/**
 * @file lagoon/utils/wx_gui_factory/arrayer.hpp
 * @brief wxWidgets-based GUI polymorphic factory arrayer
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_WX_GUI_FACTORY_ARRAYER_1103101415_HPP
#define LAGOON_UTILS_WX_GUI_FACTORY_ARRAYER_1103101415_HPP

#include <cassert>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class SourceTraits>
class wx_gui_factory_arrayer
 : public wx_gui_factory_utils
 , public polymorph_factory_arrayer
{
private:
	SourceTraits source_traits;

	typedef typename polymorph_factory_arrayer::element_producer
		element_producer;
public:
	wx_gui_factory_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	)
	{
		//TODO
		assert(!"Initializer list construction not implemented yet");
	}

	~wx_gui_factory_arrayer(void)
	{
	}

	raw_ptr data(void)
	{
		//TODO:
		return raw_ptr();
	}

	void finish(raw_ptr parent_data)
	{
	}

	void create(element_producer&)
	{
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

