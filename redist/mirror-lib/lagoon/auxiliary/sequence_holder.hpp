/**
 *  .file lagoon/auxiliary/sequence_holder.hpp
 *  .brief Implementation of an internal helper class for initializing
 *   and creating run-time meta-object containers.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_SEQUENCE_HOLDER_1011291729_HPP
#define LAGOON_AUX_SEQUENCE_HOLDER_1011291729_HPP

#include <lagoon/auxiliary/range_maker.hpp>
#include <mirror/meta_prog.hpp>

#include <type_traits>
#include <vector>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Helper class for creating and holding a sequence of meta-objects
// extracted from the MetaObjects by the RangeExtractor template
template <
	class MetaObject,
	class Interface,
	template <class> class RangeExtractor
>
class sequence_holder
{
private:
	template <typename ItemList, typename SharedItems>
	static void initialize(
		SharedItems& items,
		std::false_type
	)
	{ }

	template <typename ItemList, typename SharedItems>
	static void initialize(
		SharedItems& items,
		std::true_type
	)
	{
		// if the container is not yet initialized
		if(!bool(items))
		{
			// make the container
			items.reset(new ItemList());
			// and fill it with meta-objects
			mirror::mp::for_each<
				typename RangeExtractor<
					MetaObject
				>::type
			>(aux::range_maker<Interface>(
				*items
			));
		}
	}
public:
	template <typename NotEmpty>
	static range<Interface> make(NotEmpty not_empty)
	{
		// the type of the container storing the meta-objects
		// reflecting the items of the base-level construct
		// by the MetaObject
		typedef std::vector< shared<Interface> > item_list;

		// a shared pointer to the container
		typedef std::shared_ptr<item_list> shared_items;

		// the shared container holding meta-objects reflecting
		// the items
		static shared_items items;

		// initialize the list if necessary
		initialize<item_list>(items, not_empty);
		// return a range for traversing the elements in the list
		return range<Interface>(
			items->begin(),
			items->end()
		);
	}

	static range<Interface> all(void)
	{
		// make a non-empty range
		return make(std::true_type());
	}

	static range<Interface> empty(void)
	{
		// make an empty range
		return make(std::false_type());
	}

};

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

