/**
 * .file mirror/utils/sdn_factory/enumerator.hpp
 * .brief Enumerator for the structured data factory plugin framework
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_ENUMERATOR_1101181542_HPP
#define MIRROR_UTILS_SDN_ENUMERATOR_1101181542_HPP

#include <mirror/utils/sdn_factory/native_source.hpp>
#include <mirror/utils/enum_val_by_name.hpp>

MIRROR_NAMESPACE_BEGIN

// Source for native types
template <class Product, class Traits>
class sdn_fact_enum
{
private:
	typedef aux::sdn_fact_param<Traits> param_type;
	typedef aux::sdn_fact_handler<Traits> handler_type;

	typedef aux::sdn_fact_native_handler<Traits> native_handler;
	typedef enum_value_by_name<Product> val_by_name;
	struct
	{
		double operator()(const std::string& str) const
		{
			return val_by_name::has(str)? 1.0: 0.0;
		}
	} matcher;

	std::shared_ptr<native_handler> handler;
public:
	template <class ConstructionInfo>
	sdn_fact_enum(
		const param_type& parent_data,
		ConstructionInfo
	): handler(new native_handler(
		parent_data,
		ConstructionInfo::parameter::type::local_name(),
		ConstructionInfo::parameter::base_name(),
		std::ref(matcher)
	))
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.handler().add_subhandler(handler.get());
	}

	inline Product operator()(void)
	{
		return val_by_name::get(handler->literal());
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

