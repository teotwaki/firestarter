/**
 * @file mirror/meta_prog/not.hpp
 * @brief The negation metafunction for boolean constant type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_NOT_1011291729_HPP
#define MIRROR_META_PROG_NOT_1011291729_HPP


#include <mirror/meta_prog/forward_decl.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

template <bool Value>
struct not_<std::integral_constant<bool, Value> >
 : public std::integral_constant<bool, !Value>
{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Negates the BooleanConstant type passed as argument
/**
 *  @tparam BooleanConstant the boolean constant to be negated
 *
 *  @ingroup meta_programming
 */
template <typename BooleanConstant>
struct not_ : public BooleanConstant
{ };
#else
template <typename X>
struct not_
 : public std::integral_constant<bool, !X::value>
{ };
#endif


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

