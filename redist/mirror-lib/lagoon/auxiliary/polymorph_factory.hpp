/**
 * .file lagoon/auxiliary/polymorph_factory.hpp
 * .brief Traits for the polymorphic factory implementation
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_POLYMORPH_FACTORY_1011291729_HPP
#define LAGOON_AUX_POLYMORPH_FACTORY_1011291729_HPP

#include <lagoon/config.hpp>
#include <lagoon/auxiliary/object_getter.hpp>
#include <lagoon/polymorph_factory.hpp>
#include <mirror/meta_type.hpp>
#include <mirror/meta_prog/range.hpp>

#if LAGOON_FACT_WITH_CREATE
#include <boost/any.hpp>
#endif

LAGOON_NAMESPACE_BEGIN
namespace aux {

class polymorph_factory_traits
{
public:
	typedef polymorph_factory factory;
	typedef polymorph_factory_manager manager;
	typedef polymorph_factory_composite composite;
	typedef polymorph_factory_arrayer arrayer;
	typedef polymorph_factory_enumerator enumerator;

	template <typename Product>
	struct manufacturer
	{
		typedef polymorph_factory_manufacturer<Product> type;
	};

	typedef polymorph_factory_suppliers suppliers;

	typedef polymorph_factory_builder builder;
	typedef raw_ptr data;

	typedef raw_ptr any_ptr;
#if LAGOON_FACT_WITH_CREATE
	typedef boost::any any_value;
#else
	typedef void any_value;
#endif

	template <typename MetaType>
	static shared<meta_type> product_meta_data(MetaType)
	{
		return aux::get_meta_object<
			MetaType,
			meta_type
		>();
	}

	template <class MetaClass, class ConstructorIndex>
	static shared<meta_constructor> constructor_meta_data(
		MetaClass meta_product,
		ConstructorIndex constructor_index
	)
	{
		typedef typename mirror::mp::at<
			typename mirror::constructors<MetaClass>::type,
			ConstructorIndex
		>::type MetaConstructor;
		return aux::get_meta_object<
			MetaConstructor,
			meta_constructor
		>();
	}

	template <class ConstructionInfo>
	static shared<meta_parameter> fn_param_meta_data(ConstructionInfo)
	{
		return aux::get_meta_object<
			typename ConstructionInfo::parameter,
			meta_parameter
		>();
	}

	template <class Context>
	static polymorph_factory_context context_data(Context ctxt)
	{
		return polymorph_factory_context(ctxt);
	}

	template <typename Product>
	static Product dereference(
		const raw_ptr& product_ptr,
		mirror::mp::identity<Product>
	)
	{
		return checked_deref(
			mirror::raw_cast<Product*>(
				product_ptr
			)
		);
	}

private:
	template <typename Product>
	static inline Product& checked_deref(Product* prod)
	{
		assert(prod != nullptr);
		return *prod;
	}
};

template <typename Product>
class polymorph_factory_
 : public mirror::polymorph_factory<
	Product,
	polymorph_factory_traits
>
{
private:
	typedef mirror::polymorph_factory<
		Product,
		polymorph_factory_traits
	> base_class;
public:
	inline polymorph_factory_(
		polymorph_factory_builder& bldr,
		raw_ptr build_data
	): base_class(bldr, build_data)
	{ }
};

} // namespace aux
LAGOON_NAMESPACE_END

#endif // include guard

