/**
 * .file mirror/utils/polymorph_factory/enumerator_holder.hpp
 * .brief polymorphic factory template for the factory generator
 *  utility
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_ENUMERATOR_HOLDER_1101181652_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_ENUMERATOR_HOLDER_1101181652_HPP

#include <mirror/factory.hpp>

#include <mirror/meta_prog/identity.hpp>

#include <memory>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

template <class Product, class SourceTraits>
class polymorph_factory_enumerator_holder
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::enumerator polymorph_enumerator;

	std::shared_ptr<polymorph_enumerator> enumerator;

	typedef polymorph_factory_data<SourceTraits> enumerator_data;
public:
	template <class ConstructionInfo>
	inline polymorph_factory_enumerator_holder(
		const enumerator_data& parent_data,
		ConstructionInfo construction_info
	): enumerator(
		parent_data.factory_builder->make_enumerator(
			parent_data.factory_data,
			source_traits.fn_param_meta_data(construction_info),
			source_traits.context_data(
				typename ConstructionInfo::context()
			)
		)
	)
	{ }

	inline void finish(const enumerator_data& parent_data)
	{
		enumerator->finish(parent_data.factory_data);
	}

	inline Product operator()(void)
	{
		return Product(enumerator->create());
	}
};

MIRROR_NAMESPACE_END

#endif // include guard

