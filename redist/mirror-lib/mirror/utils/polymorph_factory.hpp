/**
 * @file mirror/utils/polymorph_factory.hpp
 * @brief polymorphic factory template for the factory generator
 * utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_1011291729_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>
#include <mirror/meta_class.hpp>

#include <mirror/meta_prog/identity.hpp>
#include <mirror/meta_prog/range.hpp>

#include <mirror/utils/native_types.hpp>
#include <mirror/utils/polymorph_factory/fwd.hpp>
#include <mirror/utils/polymorph_factory/data.hpp>
#include <mirror/utils/polymorph_factory/manager_holder.hpp>
#include <mirror/utils/polymorph_factory/manufacturer_holder.hpp>
#include <mirror/utils/polymorph_factory/enumerator_holder.hpp>
#include <mirror/utils/polymorph_factory/suppliers_holder.hpp>
#include <mirror/utils/polymorph_factory/arrayer_holder.hpp>

#include <cassert>

MIRROR_NAMESPACE_BEGIN

template <typename FactoryTraits>
struct polymorphic_factory_maker
 :  public mirror::factory_maker<
	mirror::polymorph_factory_manufacturer_holder,
	mirror::polymorph_factory_suppliers_holder,
	mirror::polymorph_factory_enumerator_holder,
	FactoryTraits
>{ };

template <typename Product, class FactoryTraits>
class polymorph_factory_base
{
private:
	typedef typename FactoryTraits::builder polymorph_builder;
	typedef typename FactoryTraits::data polymorph_data;
	typedef typename FactoryTraits::composite polymorph_composite;

	typedef typename aux::reflect_type<Product>::type meta_product;

	FactoryTraits traits;
protected:
	polymorph_builder& builder;

	polymorph_factory_data<FactoryTraits> root_data;

	std::shared_ptr<polymorph_composite> composite;

	polymorph_factory_data<FactoryTraits> data;

	polymorph_factory_base(
		polymorph_builder& bldr,
		polymorph_data build_data
	): builder(bldr)
	 , root_data(&builder, build_data)
	 , composite(
		builder.make_composite(
			build_data,
			traits.product_meta_data(meta_product()),
			traits.context_data(mp::empty_range())
		)
	), data(root_data, composite->data())
	{ }
public:
	typedef typename polymorphic_factory_maker<
		FactoryTraits
	>:: template factory<Product>::type factory_type;
};

template <typename Product, class FactoryTraits, class NoConstructors>
class polymorph_factory_impl;

template <typename Product, class FactoryTraits>
class polymorph_factory_impl<Product, FactoryTraits, std::false_type>
 : public polymorph_factory_base<Product, FactoryTraits>
 , public FactoryTraits::factory
{
private:
	typedef polymorph_factory_base<Product, FactoryTraits> base;
	typedef typename FactoryTraits::builder polymorph_builder;
	typedef typename FactoryTraits::data polymorph_data;
	typedef typename FactoryTraits::any_ptr any_ptr;
	typedef typename FactoryTraits::any_value any_value;

	typename base::factory_type base_factory;
public:
	polymorph_factory_impl(
		polymorph_builder& bldr,
		polymorph_data build_data
	): base(bldr, build_data)
	 , base_factory(this->data)
	{
		this->composite->finish(build_data);
	}

	any_ptr new_(void)
	{
		this->composite->on_create();
		return any_ptr(base_factory.new_());
	}
private:
	static any_value do_create(mp::identity<void>);

	template <typename T>
	inline any_value do_create(mp::identity<T>)
	{
		this->composite->on_create();
		return any_value(base_factory());
	}
public:
	any_value create(void)
	{
		return do_create(mp::identity<any_value>());
	}
};

template <typename Product, class FactoryTraits>
class polymorph_factory_impl<Product, FactoryTraits, std::true_type>
 : public FactoryTraits::factory
{
private:
	typedef typename FactoryTraits::builder polymorph_builder;
	typedef typename FactoryTraits::data polymorph_data;
	typedef typename FactoryTraits::any_ptr any_ptr;
	typedef typename FactoryTraits::any_value any_value;
public:
	polymorph_factory_impl(
		polymorph_builder& bldr,
		polymorph_data build_data
	){ }

	any_ptr new_(void)
	{
		return any_ptr();
	}

	any_value create(void)
	{
		return any_value();
	}
};

template <typename Product, class FactoryTraits>
class polymorph_factory
 : public polymorph_factory_impl<
	Product,
	FactoryTraits,
	typename mirror::mp::empty<
		typename mirror::constructors<
			MIRRORED_TYPE(Product)
		>::type
	>::type
>
{
private:
	typedef typename FactoryTraits::builder polymorph_builder;
	typedef typename FactoryTraits::data polymorph_data;
	typedef polymorph_factory_impl<
		Product,
		FactoryTraits,
		typename mirror::mp::empty<
			typename mirror::constructors<
				MIRRORED_TYPE(Product)
			>::type
		>::type
	> base_class;
public:
	inline polymorph_factory(
		polymorph_builder& bldr,
		polymorph_data build_data
	): base_class(bldr, build_data)
	{ }
};

MIRROR_NAMESPACE_END

#endif // include guard

