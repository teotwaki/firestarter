/**
 * @file mirror/meta_prog/and.hpp
 * @brief The and metafunction for boolean constant type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_AND_1011291729_HPP
#define MIRROR_META_PROG_AND_1011291729_HPP


#include <mirror/meta_prog/forward_decl.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

// Single-parameter and_ meta function specialization
template <class Bool>
struct and_<Bool>
 : public ::std::integral_constant<bool, Bool::value>
{ };

// Multi-parameter and_ meta function specialization
template <class Bool, class ... Bools>
struct and_<Bool, Bools...>
 : public ::std::integral_constant<
	bool,
	Bool::value && and_<Bools...>::value
>{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns logical AND of the BooleanConstants type passed as arguments
/**
 *  @tparam BooleanConstants the boolean constants to be AND-ed
 *
 *  @ingroup meta_programming
 */
template <typename ... BooleanConstants>
struct and_ : public BooleanConstant
{ };
#endif


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

