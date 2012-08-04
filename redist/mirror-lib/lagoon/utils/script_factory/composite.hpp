/**
 * .file lagoon/utils/script_factory/composite.hpp
 * .brief Script-parsing polymorphic factory composite
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_COMPOSITE_1011291729_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_COMPOSITE_1011291729_HPP

#include <cassert>
#include <memory>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class script_fact_composite
 : public polymorph_factory_composite
 , public aux::script_fact_utils<Traits>
{
private:
	typedef mirror::aux::script_fact_param<Traits> param_type;
	typedef mirror::aux::script_fact_parser<Traits> parser_type;
	typedef mirror::aux::script_fact_proxy_parser<Traits> proxy_parser;

	std::shared_ptr<proxy_parser> proxy;
	param_type param;
public:
	script_fact_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): proxy(new proxy_parser)
	 , param(proxy.get(), this->deref(parent_data))
	{ }

	script_fact_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): proxy(new proxy_parser)
	 , param(proxy.get(), this->deref(parent_data))
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).parser().add_subparser(proxy.get());
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

