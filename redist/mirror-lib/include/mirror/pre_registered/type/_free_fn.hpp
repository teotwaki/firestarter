/**
 *  @file mirror/pre_registered/type/_free_fn.hpp
 *  @brief meta_type specialization for free function types
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_FREE_FN_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_FREE_FN_1011291729_HPP

#include <mirror/meta_type.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/auxiliary/calling_convention.hpp>
#include <mirror/auxiliary/function_type_name.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename RetVal, typename ... Params>
struct meta_type<RetVal (Params...)>
 : aux::function_type_name<
	RetVal,
	calling_convention::__default_,
	mp::range<Params...>
>, aux::non_trivial_meta_type<RetVal>
{
	typedef typename meta_type<RetVal>::original_type
		(original_type)(Params...);
};


// TODO: this needs to be finished
#define MIRROR_IMPLEMENT_FREE_FUNCTION_META_TYPE(CALLING_CONV) \
template <typename RetVal, typename ... Params> \
struct meta_type<RetVal (__##CALLING_CONV) (Params...)> \
 : aux::function_type_name< \
	RetVal, \
	calling_convention::__##CALLING_CONV##_, \
	mp::range<Params...> \
>, aux::non_trivial_meta_type<RetVal> \
{ \
	typedef typename meta_type<RetVal>::_type_category \
		_type_category;\
	typedef typename meta_type<void>::scope scope; \
	typedef typename meta_type<RetVal>::original_type \
		(__##CALLING_CONV original_type)(Params...); \
};

//MIRROR_IMPLEMENT_FREE_FUNCTION_META_TYPE(cdecl)
//MIRROR_IMPLEMENT_FREE_FUNCTION_META_TYPE(pascal)

#undef MIRROR_IMPLEMENT_FREE_FUNCTION_META_TYPE


MIRROR_NAMESPACE_END

#endif //include guard

