/**
 * @file mirror/utils/script_factory.hpp
 * @brief Framework for creating script parsing factory generator plugins
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_FACTORY_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>

#include <mirror/using_directive.hpp>
// script factory-related
#include <mirror/utils/native_types.hpp>
#include <mirror/utils/script_factory/fwd_decl.hpp>
#include <mirror/utils/script_factory/default_traits.hpp>
#include <mirror/utils/script_factory/parsers.hpp>
#include <mirror/utils/script_factory/suppliers.hpp>
#include <mirror/utils/script_factory/default_source.hpp>
#include <mirror/utils/script_factory/native_source.hpp>
#include <mirror/utils/script_factory/initlist_source.hpp>
#include <mirror/utils/script_factory/enumerator.hpp>
#include <mirror/utils/script_factory/manager.hpp>
#include <mirror/utils/script_factory/data.hpp>

#include <initializer_list>

MIRROR_NAMESPACE_BEGIN

template <class Product, class Traits>
class script_fact_manuf
 : public aux::script_fact_source<
	Product,
	Traits,
	typename is_util_native_type<Product>::type
>
{
private:
	typedef aux::script_fact_source<
		Product,
		Traits,
		typename is_util_native_type<Product>::type
	> base_class;
	typedef aux::script_fact_param<Traits> param_type;
public:
	//TODO: this can be replaced with inherited constructors
	template <class ConstructionInfo>
	inline script_fact_manuf(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): base_class(parent_data, construction_info)
	{ }
};

template <class Element, class Traits>
class script_fact_manuf<
	std::initializer_list<Element>,
	Traits
> : public aux::script_fact_seq_source<Element, Traits>
{
private:
	typedef aux::script_fact_seq_source<
		Element,
		Traits
	> base_class;
	typedef aux::script_fact_param<Traits> param_type;
public:
	//TODO: this can be replaced with inherited constructors
	template <class ConstructionInfo>
	inline script_fact_manuf(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): base_class(parent_data, construction_info)
	{ }
};

typedef factory_maker<
	script_fact_manuf,
	script_fact_suppl,
	script_fact_enum,
	script_fact_def_traits<std::string::const_iterator>
> script_factory_maker;

typedef factory_maker<
	script_fact_manuf,
	script_fact_suppl,
	script_fact_enum,
	script_fact_def_traits<const char*>
> c_str_script_factory_maker;

MIRROR_NAMESPACE_END

#endif //include guard

