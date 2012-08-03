/**
 * .file mirror/utils/polymorph_factory/suppliers_holder.hpp
 * .brief suppliers holder for the polymorphic factory template
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_SUPPLIERS_HOLDER_1011291729_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_SUPPLIERS_HOLDER_1011291729_HPP

#include <mirror/meta_prog/identity.hpp>

#include <memory>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

// Implementation of suppliers
template <class Product, class SourceTraits>
class polymorph_factory_suppliers_holder
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::suppliers polymorph_suppliers;
	std::shared_ptr<polymorph_suppliers> suppliers;

	typedef polymorph_factory_data<SourceTraits> suppliers_data;
public:
	template <class ConstructionInfo>
	inline polymorph_factory_suppliers_holder(
		const suppliers_data& parent_data,
		ConstructionInfo construction_info
	): suppliers(
		parent_data.factory_builder->make_suppliers(
			parent_data.factory_data,
			source_traits.fn_param_meta_data(construction_info),
			source_traits.context_data(
				typename ConstructionInfo::context()
			)
		)
	)
	{ }

	inline void finish(const suppliers_data& parent_data)
	{
		suppliers->finish(parent_data.factory_data);
	}

	inline Product operator()(void)
	{
		return source_traits.dereference(
			suppliers->get(),
			mp::identity<Product>()
		);
	}
};

MIRROR_NAMESPACE_END

#endif // include guard

