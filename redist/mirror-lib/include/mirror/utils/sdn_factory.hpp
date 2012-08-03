/**
 * @file mirror/utils/sdn_factory.hpp
 * @brief Framework for creating structured data notation factory plugins
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_FACTORY_1011291729_HPP
#define MIRROR_UTILS_SDN_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>

#include <mirror/using_directive.hpp>
// script factory-related
#include <mirror/utils/native_types.hpp>
#include <mirror/utils/sdn_factory/fwd_decl.hpp>
#include <mirror/utils/sdn_factory/error.hpp>
#include <mirror/utils/sdn_factory/default_traits.hpp>
#include <mirror/utils/sdn_factory/handlers.hpp>
#include <mirror/utils/sdn_factory/suppliers.hpp>
#include <mirror/utils/sdn_factory/default_source.hpp>
#include <mirror/utils/sdn_factory/native_source.hpp>
#include <mirror/utils/sdn_factory/initlist_source.hpp>
#include <mirror/utils/sdn_factory/enumerator.hpp>
#include <mirror/utils/sdn_factory/manager.hpp>
#include <mirror/utils/sdn_factory/data.hpp>

MIRROR_NAMESPACE_BEGIN

template <class Product, class Traits>
class sdn_fact_manuf
 : public aux::sdn_fact_source<
	Product,
	Traits,
	typename is_util_native_type<Product>::type
>
{
private:
	typedef aux::sdn_fact_source<
		Product,
		Traits,
		typename is_util_native_type<Product>::type
	> base_class;
	typedef aux::sdn_fact_param<Traits> param_type;
public:
	//TODO: this can be replaced with inherited constructors
	template <class ConstructionInfo>
	inline sdn_fact_manuf(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): base_class(parent_data, construction_info)
	{ }
};

template <class Element, class Traits>
class sdn_fact_manuf<
	std::initializer_list<Element>,
	Traits
> : public aux::sdn_fact_seq_source<Element, Traits>
{
private:
	typedef aux::sdn_fact_seq_source<
		Element,
		Traits
	> base_class;
	typedef aux::sdn_fact_param<Traits> param_type;
public:
	//TODO: this can be replaced with inherited constructors
	template <class ConstructionInfo>
	inline sdn_fact_manuf(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): base_class(parent_data, construction_info)
	{ }
};

MIRROR_NAMESPACE_END

#endif //include guard

