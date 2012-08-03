/**
 * .file mirror/utils/polymorph_factory/manager_holder.hpp
 * .brief manager holder for the polymorphic factory template
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_MANAGER_HOLDER_1011291729_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_MANAGER_HOLDER_1011291729_HPP

#include <mirror/utils/polymorph_factory/data.hpp>

#include <memory>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

// Manager
template <class SourceTraits>
class polymorph_factory_manufacturer_holder<void, SourceTraits>
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::manager polymorph_manager;
	std::shared_ptr<polymorph_manager> manager;

	typedef polymorph_factory_data<SourceTraits> manager_data;
	manager_data data;
public:
	template <class Context>
	polymorph_factory_manufacturer_holder(
		const manager_data& parent_data,
		Context context
	): manager(
		parent_data.factory_builder->make_manager(
			parent_data.factory_data,
			source_traits.context_data(context)
		)
	), data(parent_data, manager->data())
	{ }

	inline void finish(const manager_data& parent_data)
	{
		manager->finish(parent_data.factory_data);
	}

	template <class ConstructorInfo>
	manager_data add_constructor(
		const manager_data& parent_data,
		ConstructorInfo constructor_info
	)
	{
		typename ConstructorInfo::product product;
		typename ConstructorInfo::constructor_index constructor_index;
		typename ConstructorInfo::context context;
		return manager_data(
			data,
			manager->add_constructor(
				parent_data.factory_data,
				source_traits.constructor_meta_data(
					product,
					constructor_index
				),
				source_traits.context_data(context),
				constructor_info.enumeration_info.going_backward()
			)
		);
	}

	inline int index(void)
	{
		return manager->index();
	}
};

MIRROR_NAMESPACE_END

#endif // include guard

