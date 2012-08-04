/**
 *  @file mirror/meta_prog/contains.hpp
 *  @brief Helper meta-function checking for the presence of a type
 *  in a range.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_CONTAINS_1011291729_HPP
#define MIRROR_META_PROG_CONTAINS_1011291729_HPP


#include <mirror/meta_prog/find.hpp>
#include <mirror/meta_prog/not.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns true if the range contains the searched type
/**
 *  @tparam Range the range to search in.
 *  @tparam Type the type searched for
 *
 *  @ingroup meta_programming
 */
template <typename Range, typename Type>
struct contains : public BooleanConstant
{ };
#endif

template <typename Range, typename T>
struct contains
 : public not_< empty < find<Range, T> > >::type
{ };

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

