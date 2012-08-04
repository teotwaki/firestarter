/**
 *  @file mirror/pre_registered/type/_ptr_ref.hpp
 *  @brief meta_type specialization for pointers and references
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_PTR_REF_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_PTR_REF_1011291729_HPP

#include <mirror/meta_type.hpp>
// type name decorations
#include <mirror/auxiliary/decorated_type_name.hpp>

MIRROR_NAMESPACE_BEGIN

// Meta-types for pointers
template <class PointeeType>
struct meta_type<PointeeType*>
 : aux::decorated_type_name<
	meta_type<PointeeType>,
	aux::type_name_decor<PointeeType*>
>, aux::non_trivial_meta_type<PointeeType>
{
	typedef typename meta_type<PointeeType>::original_type*
		original_type;
};

// Meta-types for references
template <class ReferredToType>
struct meta_type<ReferredToType&>
 : aux::decorated_type_name<
	meta_type<ReferredToType>,
	aux::type_name_decor<ReferredToType&>
>, aux::non_trivial_meta_type<ReferredToType>
{
	typedef typename meta_type<ReferredToType>::original_type&
		original_type;
};

// Meta-types for r-value references
template <class ReferredToType>
struct meta_type<ReferredToType&&>
 : aux::decorated_type_name<
	meta_type<ReferredToType>,
	aux::type_name_decor<ReferredToType&&>
>, aux::non_trivial_meta_type<ReferredToType>
{
	typedef typename meta_type<ReferredToType>::original_type&&
		original_type;
};

MIRROR_NAMESPACE_END

#endif //include guard

