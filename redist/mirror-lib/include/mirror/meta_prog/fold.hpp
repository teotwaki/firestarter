/**
 * @file mirror/meta_prog/fold.hpp
 * @brief The range folding meta-function
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_FOLD_1011291729_HPP
#define MIRROR_META_PROG_FOLD_1011291729_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/lambda.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the result of successive application of ForwardOp on the range
/** This meta-function returns the result of successive application
 *  of the binary forward operation on the status and all the items
 *  in the range passed as argument.
 *
 *  @tparam Range the range to traverse
 *  @tparam Status the initial status for the first call of the @a ForwardOp
 *  @tparam ForwardOp the operation to be executed during the traversal
 *
 *  @see for_each
 *  @ingroup meta_programming
 */
template <
	typename Range,
	typename Status,
	typename ForwardOp
> struct fold
{
	/// The result of the operation
	typedef unspecified_type type;
};
#endif

template <
	typename X,
	typename Status,
	class ForwardOp
> struct fold
{
	MIRROR_ASSERT_RETURNS_RANGE(X);

	typedef typename fold<
		typename X::type,
		Status,
		ForwardOp
	>::type type;
};

template <typename Status, class ForwardOp>
struct fold<range<>, Status, ForwardOp>
{
	typedef Status type;
};

template <
	typename T,
	typename Status,
	class ForwardOp
> struct fold<range<T>, Status, ForwardOp>
{
	typedef typename apply<
		ForwardOp,
		Status,
		T
	>::type type;
};

template <
	typename T,
	typename ... P,
	typename Status,
	class ForwardOp
> struct fold<range<T, P...>, Status, ForwardOp>
{
	typedef typename fold<
		range<P...>,
		typename apply<
			ForwardOp,
			Status,
			T
		>::type,
		ForwardOp
	>::type type;
};


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

