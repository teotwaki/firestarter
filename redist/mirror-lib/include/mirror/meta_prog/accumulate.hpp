/**
 * @file mirror/meta_prog/accumulate.hpp
 * @brief The accumulate run-time function for range elements
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_ACCUMULATE_1103161408_HPP
#define MIRROR_META_PROG_ACCUMULATE_1103161408_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/traits.hpp>
#include <mirror/meta_prog/iter_info.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {
namespace aux {

// Implementation of accumulate for empty ranges
template <
	typename ResultType,
	typename Functor,
	typename ValueType,
	typename IsFirst,
	typename IsLast
>
ResultType accumulate(
	Functor func,
	ValueType final,
	range<>,
	IsFirst is_first,
	IsLast is_last
)
{
	return final;
}

// Implementation of accumulate for single element ranges
template <
	typename ResultType,
	typename T,
	typename Functor,
	typename ValueType,
	typename IsFirst,
	typename IsLast
>
ResultType accumulate(
	Functor func,
	ValueType value,
	range<T>,
	IsFirst is_first,
	IsLast is_last
)
{
	// call the functor on the last element
	return func(value, iteration_info<T, IsFirst, std::true_type>());
}

// Implementation of accumulate for multi element ranges
template <
	typename ResultType,
	typename T,
	typename ... P,
	typename Functor,
	typename ValueType,
	typename IsFirst,
	typename IsLast
>
ResultType accumulate(
	Functor func,
	ValueType value,
	range<T, P...>,
	IsFirst is_first,
	IsLast is_last
)
{
	return accumulate<ResultType>(
		func,
		func(value, iteration_info<T, IsFirst, IsLast>()),
		range<P...>(),
		std::false_type(),
		is_last
	);
}

/* Implementation which tries to use the R type as
 * a range returning meta-function
 */
template <
	typename ResultType,
	typename X,
	typename Functor,
	typename ValueType,
	typename IsFirst,
	typename IsLast
>
ResultType accumulate(
	Functor func,
	ValueType value,
	X,
	IsFirst is_first,
	IsLast is_last
)
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	return accumulate<ResultType>(
			func,
			value,
			typename X::type(),
			is_first,
			is_last
	);
}

template <class ResultType, class Functor>
struct accumulate_ii_remover
{
	Functor func;

	inline accumulate_ii_remover(Functor f)
	 : func(f)
	{ }

	template <typename ValueType, class IterInfo>
	inline ResultType operator()(ValueType value, IterInfo) const
	{
		return func(value, typename IterInfo::type());
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
template <
	typename Range,
	typename ResultType,
	typename Functor,
	typename ValueType
> ResultType accumulate(Functor func, ValueType initial)
{
	return aux::accumulate<ResultType>(
		aux::accumulate_ii_remover<ResultType, Functor>(func),
		initial,
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
template <
	typename Range,
	typename ResultType,
	typename Functor,
	typename ValueType
> ResultType accumulate_ii(Functor func, ValueType initial)
{
	return aux::accumulate<ResultType>(
		func,
		initial,
		Range(),
		std::true_type(),
		std::false_type()
	);
}

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

