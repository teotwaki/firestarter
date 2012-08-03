/**
 *  @file mirror/meta_prog/unique.hpp
 *  @brief Meta-function returning a range which contains the elements
 *  of the passed ranges without duplicities.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_UNIQUE_1011291729_HPP
#define MIRROR_META_PROG_UNIQUE_1011291729_HPP


#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a range with the duplicities removed
/**
 *  @tparam Range the range to be filtered
 *
 *  @ingroup meta_programming
 */
template <class Range>
struct unique
{
	/// The range containing only the unique elements
	typedef UniqueRange type;
};
#endif

namespace aux {

// declaration of the internal helper for the unique meta-function
template <class UniqueRange, class TailRange>
struct unique_helper;

// specialization ending the filtering
template <typename ... UniqueP>
struct unique_helper<range<UniqueP...>, range<> >
{
	typedef range<UniqueP...> type;
};

// specialization of the unique_helper for non-empty
template <typename ... UniqueP, typename T, typename ... P>
struct unique_helper<range<UniqueP...>, range<T, P...> >
{
	typedef typename if_<
		typename empty<find<range<UniqueP...>, T> >::type,
		typename unique_helper<
			range<UniqueP..., T>,
			range<P...>
		>::type,
		typename unique_helper<
			range<UniqueP...>,
			range<P...>
		>::type
	>::type type;
};

} // namespace aux

// Implementation of the unique meta-function
template <typename ... P>
struct unique<range<P...> >
{
	// use the internale helper to remove the duplicities
	typedef typename aux::unique_helper<
		// at first the unique range is empty
		range<>,
		// and the remaining range is the range passes
		// as template parameter
		range<P...>
	>::type type;
};


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

