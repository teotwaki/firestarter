/**
 * .file lagoon/utils/sdn_factory/manufacturer.hpp
 * .brief Script-parsing polymorphic factory manufacturers
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SDN_FACTORY_MANUFACTURER_1011291729_HPP
#define LAGOON_UTILS_SDN_FACTORY_MANUFACTURER_1011291729_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Implementation of the polymorph factory manufacturer interface
template <class Product, class Traits>
class sdn_fact_manuf_native
 : public polymorph_factory_manufacturer<Product>
 , public aux::sdn_fact_utils<Traits>
{
private:
	typedef mirror::aux::sdn_fact_param<Traits> param_type;
	typedef mirror::aux::sdn_fact_handler<Traits> handler_type;
	typedef mirror::aux::sdn_fact_native_handler<Traits> native_handler;

	mirror::lex_match<Product> matcher;
	std::shared_ptr<native_handler> handler;
public:
	sdn_fact_manuf_native(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): handler(
		new native_handler(
			this->deref(parent_data),
			param->type()->full_name(),
			param->base_name(),
			std::ref(matcher)
		)
	)
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).handler().add_subhandler(handler.get());
	}

	Product create(void)
	{
		mirror::lex_conv<std::string, Product> conv;
		return conv(handler->literal());
	}
};

} // namespace aux

template <class Product, class Traits>
class sdn_fact_manufacturer
 : public aux::sdn_fact_manuf_native <Product, Traits>
{
public:
	sdn_fact_manufacturer(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): aux::sdn_fact_manuf_native<Product, Traits>(
		parent_data,
		param,
		context
	){ }
};

LAGOON_NAMESPACE_END

#endif //include guard

