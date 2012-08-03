/**
 * @file mirror/meta_prog/is_one_of.hpp
 * @brief Boolean meta-function checking if a MetaObject is a model
 * of a specified concept
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_IS_ONE_OF_1102211340_HPP
#define MIRROR_META_PROG_IS_ONE_OF_1102211340_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/is_a.hpp>
#include <mirror/meta_prog/or.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a boolean constant based on if the MetaObject is one of the Concepts
/**
 *  @tparam MetaObject the examined meta-object
 *  @tparam Concepts a range of concepts
 *
 *  @ingroup meta_programming
 */
template <class MetaObject, class Concepts>
struct is_one_of
{
	/// The @c std::true_type or the @c std::false_type
	typedef BooleanConstant type;
};
#else

template <class X, class ... Concept>
struct is_one_of<X, range<Concept ...> >
 : or_<typename is_a< X, Concept >::type ...>::type
{ };
#endif

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

