/**
 * .file mirror/utils/polymorph_factory/manufacturer_holder.hpp
 * .brief polymorphic factory template for the factory generator
 *  utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_MANUFACTURER_HOLDER_1011291729_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_MANUFACTURER_HOLDER_1011291729_HPP

#include <mirror/factory.hpp>

#include <mirror/meta_prog/identity.hpp>

#include <memory>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

// Forward declaration of manufacturer helper base class
template <class Product, class SourceTraits, class NativeType>
class polymorph_factory_manufacturer_holder_base;

// Manufacturer implementation for native types
template <class Product, class SourceTraits>
class polymorph_factory_manufacturer_holder_base<
	Product,
	SourceTraits,
	std::true_type
>
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::template manufacturer<Product>::type
		polymorph_manufacturer;

	std::shared_ptr<polymorph_manufacturer> manufacturer;

	typedef polymorph_factory_data<SourceTraits> manufacturer_data;
public:
	template <class ConstructionInfo>
	inline polymorph_factory_manufacturer_holder_base(
		const manufacturer_data& parent_data,
		ConstructionInfo construction_info
	): manufacturer(
		parent_data.factory_builder->make_manufacturer(
			parent_data.factory_data,
			source_traits.fn_param_meta_data(construction_info),
			source_traits.context_data(
				typename ConstructionInfo::context()
			),
			mp::identity<Product>()
		)
	)
	{ }

	inline void finish(const manufacturer_data& parent_data)
	{
		manufacturer->finish(parent_data.factory_data);
	}

	inline Product operator()(void)
	{
		return manufacturer->create();
	}
};

// Implementation of manufacturer for elaborated types
template <class Product, class SourceTraits>
class polymorph_factory_manufacturer_holder_base<
	Product,
	SourceTraits,
	std::false_type
>
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::composite polymorph_composite;
	std::shared_ptr<polymorph_composite> composite;

	typedef polymorph_factory_data<SourceTraits> manufacturer_data;
	manufacturer_data data;

	typedef typename mirror::factory_maker<
		mirror::polymorph_factory_manufacturer_holder,
		mirror::polymorph_factory_suppliers_holder,
		mirror::polymorph_factory_enumerator_holder,
		SourceTraits
	> make;

	typename make::template factory<Product>::type fact;
public:
	template <class ConstructionInfo>
	inline polymorph_factory_manufacturer_holder_base(
		const manufacturer_data& parent_data,
		ConstructionInfo construction_info
	): composite(
		parent_data.factory_builder->make_composite(
			parent_data.factory_data,
			source_traits.fn_param_meta_data(construction_info),
			source_traits.context_data(
				typename ConstructionInfo::context()
			)
		)
	), data(parent_data, composite->data())
	 , fact(data, construction_info)
	{ }

	inline void finish(const manufacturer_data& parent_data)
	{
		composite->finish(parent_data.factory_data);
	}

	inline Product operator()(void)
	{
		composite->on_create();
		return fact();
	}
};

// Implementation of manufacturer
template <class Product, class SourceTraits>
class polymorph_factory_manufacturer_holder
 : public polymorph_factory_manufacturer_holder_base<
	Product,
	SourceTraits,
	typename is_util_native_type<Product>::type
>
{
private:
	typedef polymorph_factory_manufacturer_holder_base<
		Product,
		SourceTraits,
		typename is_util_native_type<Product>::type
	> base_class;
	typedef polymorph_factory_data<SourceTraits> manufacturer_data;
public:
	//TODO: this can be updated to use inherited constructors
	// once they are supported by the compilers
	template <class ConstructionInfo>
	inline polymorph_factory_manufacturer_holder(
		const manufacturer_data& parent_data,
		ConstructionInfo construction_info
	): base_class(parent_data, construction_info)
	{ }
};

MIRROR_NAMESPACE_END

#endif // include guard

