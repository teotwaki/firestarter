/**
 * @file mirror/meta_prog/if.hpp
 * @brief The if-else metafunction for branching based on boolean constant type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_IF_1011291729_HPP
#define MIRROR_META_PROG_IF_1011291729_HPP


#include <mirror/meta_prog/forward_decl.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

template <class IfTrue, class IfFalse>
struct if_<std::true_type, IfTrue, IfFalse>
{
	typedef IfTrue type;
};

template <class IfTrue, class IfFalse>
struct if_<std::false_type, IfTrue, IfFalse>
{
	typedef IfFalse type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the IfTrue or the IfFalse type based on the passed Boolean type
/**
 *  @tparam BooleanConstant the boolean constant type determining the result
 *  @tparam IfTrue the type returned if the BooleanConstant has the true value
 *  @tparam IfFalse the type returned if the BooleanConstant has false value
 *
 *  @ingroup meta_programming
 */
template <class BooleanConstant, class IfTrue, class IfFalse>
struct if_
{
	/// The @a IfTrue or @a IfFalse type based on @a BooleanConstant
	typedef unspecified type;
};
#else
template <typename X, class IfTrue, class IfFalse>
struct if_ : public if_<
	std::integral_constant<bool, X::type::value>,
	IfTrue,
	IfFalse
>
{ };
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the IfTrue or the IfFalse type based on the passed boolean value
/**
 *  @tparam BooleanConstant the boolean constant value determining the result
 *  @tparam IfTrue the type returned if the BooleanConstant has the true value
 *  @tparam IfFalse the type returned if the BooleanConstant has false value
 *
 *  @ingroup meta_programming
 */
template <bool BooleanConstant, class IfTrue, class IfFalse>
struct if_c
{
	/// The @a IfTrue or @a IfFalse type based on @a BooleanConstant
	typedef unspecified type;
};
#else
template <bool Value, class IfTrue, class IfFalse>
struct if_c : public if_<
	std::integral_constant<bool, Value>,
	IfTrue,
	IfFalse
>
{ };
#endif


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

