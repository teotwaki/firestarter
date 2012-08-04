/**
 *  @file mirror/pre_registered/type/_array.hpp
 *  @brief meta_type specialization for arrays
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_ARRAY_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_ARRAY_1011291729_HPP

#include <mirror/meta_type.hpp>
// type name decorations
#include <mirror/auxiliary/decorated_type_name.hpp>

MIRROR_NAMESPACE_BEGIN

// Meta-types for arrays
template <class ElementType, size_t Size>
struct meta_type<ElementType[Size]>
 : aux::decorated_type_name<
	meta_type<ElementType>,
	aux::type_name_decor<ElementType[Size]>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef typename meta_type<ElementType>::original_type
		original_type[Size];
};

// Meta-types for arrays of const elements
template <class ElementType, size_t Size>
struct meta_type<const ElementType[Size]> : aux::decorated_type_name<
	meta_type<const ElementType>,
	aux::type_name_decor<ElementType[Size]>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef const typename meta_type<ElementType>::original_type
		original_type[Size];
};

// Meta-types for arrays of volatile elements
template <class ElementType, size_t Size>
struct meta_type<volatile ElementType[Size]> : aux::decorated_type_name<
	meta_type<volatile ElementType>,
	aux::type_name_decor<ElementType[Size]>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef volatile typename meta_type<ElementType>::original_type
		original_type[Size];
};

// Meta-types for arrays of const volatile elements
template <class ElementType, size_t Size>
struct meta_type<const volatile ElementType[Size]>
: aux::decorated_type_name<
	meta_type<const volatile ElementType>,
	aux::type_name_decor<ElementType[Size]>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef const volatile typename meta_type<ElementType>::original_type
		original_type[Size];
};

// Meta-types for arrays without known extent
template <class ElementType>
struct meta_type<ElementType[]> : aux::decorated_type_name<
	meta_type<ElementType>,
	aux::type_name_decor<ElementType[]>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef typename meta_type<ElementType>::original_type
		original_type[];
};

template <class ElementType>
struct meta_type<const ElementType []> : aux::decorated_type_name<
	meta_type<const ElementType>,
	aux::type_name_decor<ElementType []>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef typename meta_type<ElementType>::original_type const
		original_type[];
};

template <class ElementType>
struct meta_type<volatile ElementType []> : aux::decorated_type_name<
	meta_type<volatile ElementType>,
	aux::type_name_decor<ElementType []>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef typename meta_type<ElementType>::original_type volatile
		original_type[];
};

template <class ElementType>
struct meta_type<const volatile ElementType []>
: aux::decorated_type_name<
	meta_type<const volatile ElementType>,
	aux::type_name_decor<ElementType []>
>, aux::non_trivial_meta_type<ElementType>
{
	typedef typename meta_type<ElementType>::original_type const volatile
		original_type[];
};


MIRROR_NAMESPACE_END

#endif //include guard

