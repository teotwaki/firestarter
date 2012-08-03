/**
 * .file mirror/utils/script_factory/default_source.hpp
 * .brief Default manufacturer for the script-parsing factory generator plugin
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_DEFAULT_SOURCE_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_DEFAULT_SOURCE_1011291729_HPP

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Product, class Traits>
class script_fact_source<
	Product,
	Traits,
	std::false_type
>
{
private:
	typedef script_fact_param<Traits> param_type;

	typedef script_fact_proxy_parser<Traits> proxy_parser;
	std::shared_ptr<proxy_parser> proxy;

	typedef typename mirror::factory_maker<
		script_fact_manuf,
		script_fact_suppl,
		script_fact_enum,
		Traits
	> maker;
	typename maker::template factory< Product >::type fact;
public:
	template <class ConstructionInfo>
	inline script_fact_source(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): proxy(new proxy_parser)
	 , fact(param_type(proxy.get(), parent_data), construction_info)
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.parser().add_subparser(proxy.get());
	}

	inline Product operator()(void)
	{
		return fact();
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

