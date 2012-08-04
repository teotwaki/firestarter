/**
 *  @file mirror/meta_prog/transform.hpp
 *  @brief Meta-function returning a range containing elements transformed
 *  by a unary function.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_TRANSFORM_1011291729_HPP
#define MIRROR_META_PROG_TRANSFORM_1011291729_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/traits.hpp>
#include <mirror/meta_prog/lambda.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a range containing elements transformed by a unary function
/**
 *  @tparam Range the range to be transformed
 *  @tparam UnaryMetaFnClass the meta-function class transforming the elements
 *
 *  @ingroup meta_programming
 */
template <class Range, class UnaryMetaFnClass>
struct transform
{
	typedef Range type;
};
#endif

template <typename ... P, class UnaryMetaFnClass>
struct transform<range<P...>, UnaryMetaFnClass>
{
	typedef range<
		typename apply<UnaryMetaFnClass, P>::type ...
	> type;
};

// specialization for range expressions
template <typename X, class Predicate>
struct transform
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename transform<typename X::type, Predicate>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

