/**
 *  .file lagoon/auxiliary/range_maker.hpp
 *  .brief Implementation of an internal helper class for initializing
 *   run-time meta-object ranges.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_RANGE_MAKER_1011291729_HPP
#define LAGOON_AUX_RANGE_MAKER_1011291729_HPP

#include <lagoon/utils.hpp>
#include <lagoon/auxiliary/object_getter.hpp>
#include <mirror/meta_object_tags.hpp>
#include <vector>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Helper functor used for constructing ranges of meta-objects
template <typename Interface>
struct range_maker
{
	// The type of the object container to be initialized
	typedef std::vector<shared<Interface> > item_list;
	item_list& items_ref;

	// constructor taking a reference to the container
	range_maker(item_list& ref)
	 : items_ref(ref)
	{ }

	// fallback do-nothing implementation for other meta-objects
	template <typename MetaObject>
	void append_item(MetaObject, std::false_type) const { }

	// implementation of append_item for common objects
	template <typename MetaObject>
	void do_append_item(MetaObject) const
	{
		// use the make function to get an instance
		// of the run-time meta-type with the selected
		// Interface
		items_ref.push_back(
			get_meta_object<
				MetaObject,
				Interface
			>()
		);
	}

	template <typename MetaObject>
	void append_item(MetaObject mo, std::true_type) const
	{
		do_append_item(mo);
	}


	// operator called for each element of compile-time meta-object range
	template <typename MetaObject>
	void operator()(MetaObject meta_object) const
	{
		// call the appropriate overload of append item based
		// on the category of the meta-object
		decltype(lagoon::aux::has_wrapper(
			mirror::categorize_meta_object(meta_object)
		)) do_append;
		append_item(meta_object, do_append);
	}
};

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

