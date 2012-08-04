/**
 *  @file mirror/pre_registered/type/_const_volatile.hpp
 *  @brief meta_type specialization for constant and volatile types
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_CONST_VOLATILE_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_CONST_VOLATILE_1011291729_HPP

#include <mirror/meta_type.hpp>
// type name decorations
#include <mirror/auxiliary/decorated_type_name.hpp>

MIRROR_NAMESPACE_BEGIN

// Meta-types for const types
template <class NonConstType>
struct meta_type<const NonConstType>
 : aux::decorated_type_name<
	meta_type<NonConstType>,
	aux::type_name_decor<const NonConstType>
>, aux::non_trivial_meta_type<NonConstType>
{
	typedef const typename meta_type<NonConstType>::original_type
		original_type;
};

// Meta-types for volatile types
template <class NonVolatileType>
struct meta_type<volatile NonVolatileType>
 : aux::decorated_type_name<
	meta_type<NonVolatileType>,
	aux::type_name_decor<volatile NonVolatileType>
>, aux::non_trivial_meta_type<NonVolatileType>
{
	typedef volatile typename meta_type<NonVolatileType>::original_type
		original_type;
};

// Meta-types for const volatile types
template <class NonCVType>
struct meta_type<const volatile NonCVType>
 : aux::decorated_type_name<
	meta_type<NonCVType>,
	aux::type_name_decor<const volatile NonCVType>
>, aux::non_trivial_meta_type<NonCVType>
{
	typedef const volatile typename meta_type<NonCVType>::original_type
		original_type;
};

MIRROR_NAMESPACE_END

#endif //include guard

