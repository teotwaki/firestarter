/**
 * .file mirror/utils/polymorph_factory/arrayer_holder.hpp
 * .brief polymorphic factory template for the factory generator
 *  utility
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_ARRAYER_HOLDER_1103091328_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_ARRAYER_HOLDER_1103091328_HPP

#include <mirror/factory.hpp>

#include <mirror/meta_prog/identity.hpp>

#include <memory>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

// Implementation of manufacturer for elaborated types
template <class Element, class SourceTraits>
class polymorph_factory_manufacturer_holder<
	std::initializer_list<Element>,
	SourceTraits
>
{
private:
	SourceTraits source_traits;

	typedef typename SourceTraits::arrayer polymorph_arrayer;
	std::shared_ptr<polymorph_arrayer> arrayer;

	typedef polymorph_factory_data<SourceTraits> manufacturer_data;
	manufacturer_data data;

	class element_producer : public polymorph_arrayer::element_producer
	{
	public:
		// TODO: this may be optimized based on Element type
		typedef std::vector<Element> container_type;
		container_type container;
	private:
		typedef typename mirror::factory_maker<
			mirror::polymorph_factory_manufacturer_holder,
			mirror::polymorph_factory_suppliers_holder,
			mirror::polymorph_factory_enumerator_holder,
			SourceTraits
		> make;

		typename make::template factory<Element>::type fact;
	public:
		template <class ConstructionInfo>
		element_producer(
			const manufacturer_data& data,
			ConstructionInfo construction_info
		): fact(data, construction_info)
		{ }

		// TOOD: this can probably be removed
		void reset(void)
		{
			container.clear();
		}

		void make_next(void)
		{
			container.push_back(fact());
		}

	} producer;
public:
	template <class ConstructionInfo>
	inline polymorph_factory_manufacturer_holder(
		const manufacturer_data& parent_data,
		ConstructionInfo construction_info
	): arrayer(
		parent_data.factory_builder->make_arrayer(
			parent_data.factory_data,
			source_traits.product_meta_data(
				typename reflected<Element>::type()
			),
			source_traits.context_data(
				typename ConstructionInfo::context()
			)
		)
	), data(parent_data, arrayer->data())
	 , producer(data, construction_info)
	{
		arrayer->assign_producer(producer);
	}

	inline void finish(const manufacturer_data& parent_data)
	{
		arrayer->finish(parent_data.factory_data);
	}

	inline typename element_producer::container_type operator()(void)
	{
		arrayer->create(producer);
		return std::move(producer.container);
	}
};

MIRROR_NAMESPACE_END

#endif // include guard

