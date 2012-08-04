/**
 * @file mirror/meta_prog/or.hpp
 * @brief The or metafunction for boolean constant type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_OR_1011291729_HPP
#define MIRROR_META_PROG_OR_1011291729_HPP


#include <mirror/meta_prog/forward_decl.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

// Single-parameter or_ meta function specialization
template <class Bool>
struct or_<Bool>
 : public ::std::integral_constant<bool, Bool::value>
{ };

// Multi-parameter or_ meta function specialization
template <class Bool, class ... Bools>
struct or_<Bool, Bools...>
 : public ::std::integral_constant<
	bool,
	Bool::value || or_<Bools...>::value
>{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns logical OR of the BooleanConstants type passed as arguments
/**
 *  @tparam BooleanConstants the boolean constants to be OR-ed
 *
 *  @ingroup meta_programming
 */
template <typename ... BooleanConstants>
struct or_ : public BooleanConstant
{ };
#endif


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

