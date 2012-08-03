/**
 *  @file mirror/type_traits/is_default_constructible.hpp
 *  @brief Trait type for detecting the presence of a default constructor
 *  of a type.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_1011291729_HPP
#define MIRROR_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

/// Trait for detection of default-constructibility of a type
/** @note This trait may fail to detect non-implicit default
 *  constructors which can throw excptions if they are not
 *  registered with Mirror.
 *
 *  @ingroup mirror_type_traits
 */
template <typename T>
struct is_default_constructible
 : std::integral_constant<
	bool,
	::std::has_trivial_default_constructor<T>::value ||
	::std::has_nothrow_default_constructor<T>::value ||
	mirror::_class::_<T>::has_default_ctr::value
>
{ };

MIRROR_NAMESPACE_END

#endif //include guard

