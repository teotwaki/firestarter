/**
 * @file mirror/meta_prog/for_each.hpp
 * @brief The for_each run-time function for range elements
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_FOR_EACH_1011291729_HPP
#define MIRROR_META_PROG_FOR_EACH_1011291729_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/traits.hpp>
#include <mirror/meta_prog/iter_info.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {
namespace aux {

// Implementation of for_each for empty ranges
template <
	typename Functor,
	typename IsFirst,
	typename IsLast
>
void for_each(
	Functor& func,
	range<>,
	IsFirst is_first,
	IsLast is_last
)
{ }

// Implementation of for_each for single element ranges
template <
	typename T,
	typename Functor,
	typename IsFirst,
	typename IsLast
>
void for_each(
	Functor& func,
	range<T>,
	IsFirst is_first,
	IsLast is_last
)
{
	// call the functor on the last element
	func(iteration_info<
		T,
		IsFirst,
		std::true_type
	>());
}

// Implementation of for_each for multi element ranges
template <
	typename T1,
	typename T2,
	typename ... P,
	typename Functor,
	typename IsFirst,
	typename IsLast
>
void for_each(
	Functor& func,
	range<T1, T2, P...>,
	IsFirst is_first,
	IsLast is_last
)
{
	// call the functor on the front element
	func(iteration_info<
		T1,
		IsFirst,
		IsLast
	>());
	// now repeat for the rest of the range
	for_each(
		func,
		range<T2, P...>(),
		std::false_type(),
		is_last
	);
}

/* Implementation which tries to use the R type as
 * a range returning meta-function
 */
template <
	typename X,
	typename Functor,
	typename IsFirst,
	typename IsLast
>
void for_each(
	Functor& func,
	X,
	IsFirst is_first,
	IsLast is_last
)
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	for_each(func, typename X::type(), is_first, is_last);
}

template <class Functor>
struct for_each_ii_remover
{
	Functor& func;

	inline for_each_ii_remover(Functor& f)
	 : func(f)
	{ }

	template <class IterInfo>
	inline void operator()(IterInfo) const
	{
		func(typename IterInfo::type());
	}
};

} // namespace aux

/// Call the unary functor on all elements of the range
/** This function calls the @a Functor passed as argument
 *  on instances of the types in the @a Range.
 *
 *  @tparam Range the range to be traversed
 *  @tparam Functor the type of the functor to be called on each element
 *  @param func the functor to be called on each element
 *
 *  @see fold
 *  @ingroup meta_programming
 */
template <typename Range, typename Functor>
void for_each(Functor func)
{
	aux::for_each_ii_remover<Functor> adapted_func(func);
	aux::for_each(
		adapted_func,
		Range(),
		std::true_type(),
		std::false_type()
	);
}

/// Call the unary functor on all elements of the range with iteration info
/** This function calls the @a Functor passed as argument
 *  on instances of iteration_info (which provides additional
 *  information about the progress of the iteration)
 *  for every type in the @a Range.
 *
 *  The iteration_info type defines the following member typenames
 *  - type : a type from the @a Range
 *  - is_first : std::true_type or std::false_type indicating
 *               the first step of iteration
 *  - is_last : std::true_type or std::false_type indicating
 *               the last step of iteration
 *
 *  @tparam Range the range to be traversed
 *  @tparam Functor the type of the functor to be called on each element
 *  @param func the functor to be called on each element
 *
 *  @see fold
 *  @ingroup meta_programming
 */
template <typename Range, typename Functor>
void for_each_ii(Functor func)
{
	aux::for_each(
		func,
		Range(),
		std::true_type(),
		std::false_type()
	);
}


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

