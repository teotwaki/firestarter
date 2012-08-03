/**
 * @file lagoon/range/sort.hpp
 * @brief Sorting the elements of the range by an ordering functor
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_SORT_1011291729_HPP
#define LAGOON_RANGE_SORT_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/range/container.hpp>
#include <lagoon/range/std_range.hpp>
#include <cassert>
#include <algorithm>

LAGOON_NAMESPACE_BEGIN

namespace aux {

// Helper base class for sorted container_range copying
// the element from the range into a container by using
// the push_back member during the construction
struct rng2cntr_push_back
{
	rng2cntr_push_back(void) = default;

	template <class Range, class Container>
	inline rng2cntr_push_back(Range range, Container& container)
	{
		while(!range.empty())
		{
			container.push_back(range.front());
			range.step_front();
		}
	}
};

// Helper class for sorted container_range sorting
// the elements by using the given strict weak ordering
// functor passed as the first argument
struct vector_sorter
{
	template <typename StrictWeakOrdering, typename Vector>
	inline vector_sorter(StrictWeakOrdering sort, Vector& vec)
	{
		std::sort(vec.begin(), vec.end(), sort);
	}
};

// Base class for the sorted range adaptor
template <typename Range>
class sorted : public container_range<
	std::vector<shared<typename range_meta_object<Range>::type> >,
	rng2cntr_push_back,
	vector_sorter
>
{
private:
	typedef container_range<
		std::vector<shared<typename range_meta_object<Range>::type> >,
		rng2cntr_push_back,
		vector_sorter
	> base_class;
public:
	template <typename SortProc>
	sorted(const Range& rng, SortProc sort)
	 : base_class(rng, sort)
	{ }
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Sorts the elements in the range by using a strict weak ordering proc
/** Returns a range containing the elements of the original range
 *  sorted by the binary strict weak ordering functor.
 *
 *  @param range the range to be sorted
 *  @param sort_proc the functor sorting the elements
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename SortProc>
UnspecifiedRange sort(Range range, SortProc sort_proc);
#else
template <class Range, typename SortProc>
inline aux::sorted<Range> sort(Range range, SortProc sort_proc)
{
	return aux::sorted<Range>(range, sort_proc);
}

// overload for unadapted Lagoon's meta-object ranges
template <class MetaObject, typename SortProc>
inline aux::sorted<range<MetaObject> >
sort(range<MetaObject> mo_range, SortProc sort_proc)
{
	return aux::sorted<range<MetaObject> >(
		0, // selector telling to use the std range's begin()/end()
		make_std_range(mo_range),
		sort_proc
	);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

