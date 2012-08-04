/**
 * .file lagoon/utils/sdn_factory/composite.hpp
 * .brief Script-parsing polymorphic factory composite
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SDN_FACTORY_COMPOSITE_1011291729_HPP
#define LAGOON_UTILS_SDN_FACTORY_COMPOSITE_1011291729_HPP

#include <cassert>
#include <memory>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sdn_fact_composite
 : public polymorph_factory_composite
 , public aux::sdn_fact_utils<Traits>
{
private:
	typedef mirror::aux::sdn_fact_param<Traits> param_type;
	typedef mirror::aux::sdn_fact_handler<Traits> handler_type;
	typedef mirror::aux::sdn_fact_struct_handler<Traits> struct_handler;

	std::shared_ptr<struct_handler> handler;
	param_type param;
public:
	sdn_fact_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): handler(
		new struct_handler(
			this->deref(parent_data),
			product->full_name()
		)
	), param(handler.get(), this->deref(parent_data))
	{ }

	sdn_fact_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): handler(
		new struct_handler(
			this->deref(parent_data),
			param->type()->full_name(),
			param->base_name()
		)
	), param(handler.get(), this->deref(parent_data))
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).handler().add_subhandler(handler.get());
	}

	raw_ptr data(void)
	{
		return raw_ptr(&param);
	}

	void on_create(void)
	{
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

