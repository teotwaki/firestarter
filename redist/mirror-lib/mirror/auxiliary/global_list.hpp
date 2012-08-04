/**
 *  @file mirror/auxiliary/global_list.hpp
 *  @brief Declaration of a compile-time linked type lists
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_GLOBAL_LIST_1011291729_HPP
#define MIRROR_AUX_GLOBAL_LIST_1011291729_HPP

#include <mirror/auxiliary/counter.hpp>
#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cntr {

// Empty global list
template <typename Selector, typename Counter>
struct global_list
{
	typedef mp::range<> type;
};

} // namespace cntr

/** @defgroup global_lists Mirror - Global type lists
 *
 *  This module contains a set of macros which allow to define new
 *  compile-time type lists, append new items to the lists during
 *  source compilation and to get the range of types in these
 *  type lists.
 */

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Declares a new global type list with the @a SELECTOR type as identifier
/** This macro declares and initializes a new empty compile time
 *  linked type list.
 *
 *  @see mp::range
 *  @see MIRROR_ADD_TO_GLOBAL_LIST
 *  @see MIRROR_GET_GLOBAL_LIST
 *  @param SELECTOR the type which servers as the global type list identifier
 *  @ingroup global_lists
 */
#define MIRROR_REGISTER_GLOBAL_LIST(SELECTOR)
#else
#define MIRROR_REGISTER_GLOBAL_LIST(SELECTOR) \
	MIRROR_REGISTER_COUNTER_SELECTOR(SELECTOR)
#endif

// Helper macro used in implementation of global lists
#define MIRROR_GET_GLOBAL_LIST_BASE_WRAPPER(SELECTOR, LUID, TYPENAME_KW) \
mirror::cntr::global_list< \
	mirror::cntr::sel< SELECTOR >, \
	MIRROR_COUNTER_PREVIOUS_COUNT_BASE( \
		SELECTOR, \
		LUID, \
		TYPENAME_KW \
	) \
>

// Helper macro used in implementation of global lists
#define MIRROR_GET_GLOBAL_LIST_BASE(SELECTOR, LUID, TYPENAME_KW) \
	TYPENAME_KW MIRROR_GET_GLOBAL_LIST_BASE_WRAPPER( \
		SELECTOR, \
		LUID, \
		TYPENAME_KW \
	)::type

#define MIRROR_ADD_TO_GLOBAL_LIST_BASE(SELECTOR, ITEM, LUID) \
MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, LUID) \
namespace cntr { \
template <> \
struct global_list< \
	mirror::cntr::sel< SELECTOR >, \
	MIRROR_COUNTER_CURRENT_COUNT(SELECTOR, LUID) \
> \
{ \
	typedef mp::push_back< \
		MIRROR_GET_GLOBAL_LIST_BASE( \
			SELECTOR, \
			LUID, \
			MIRROR_PP_EMPTY() \
		), \
		ITEM \
	>::type type; \
}; \
}


/// Expands into a compile-time type list identified by the @a SELECTOR
/**
 *  @see MIRROR_REGISTER_GLOBAL_LIST
 *  @see MIRROR_ADD_TO_GLOBAL_LIST
 *  @see mp::range
 *  @param SELECTOR the type serving as the type list identifier
 *  @ingroup global_lists
 */
#define MIRROR_GET_GLOBAL_LIST(SELECTOR) \
	MIRROR_GET_GLOBAL_LIST_BASE( \
		SELECTOR, \
		MIRROR_COUNTER_LUID, \
		BOOST_PP_EMPTY() \
	)

#ifdef MIRROR_DOCUMENTATION_ONLY

/// Appends a new @a ITEM into the type list identified by the @a SELECTOR
/**
 *  @see MIRROR_REGISTER_GLOBAL_LIST
 *  @see MIRROR_GET_GLOBAL_LIST
 *  @param SELECTOR the identifier of the compile-time type list
 *  @param ITEM the item to be added to the compile-time type list
 *  @ingroup global_lists
 */
#define MIRROR_ADD_TO_GLOBAL_LIST(SELECTOR, ITEM)
#else

#ifndef MIRROR_COUNTER_UNRELIABLE

#define MIRROR_ADD_TO_GLOBAL_LIST(SELECTOR, ITEM) \
	MIRROR_ADD_TO_GLOBAL_LIST_BASE( \
		SELECTOR, \
		ITEM, \
		MIRROR_COUNTER_LUID \
	)

#else // MIRROR_COUNTER_UNRELIABLE

// if we don't have a reliable counter adding to
// global lists is disabled
#define MIRROR_ADD_TO_GLOBAL_LIST(SELECTOR, ITEM)

#define MIRROR_NO_GLOBAL_LISTS

#endif //  MIRROR_COUNTER_UNRELIABLE
#endif // !MIRROR_DOCUMENTATION_ONLY

MIRROR_NAMESPACE_END

#endif //include guard

