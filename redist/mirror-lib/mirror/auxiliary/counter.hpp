/**
 *  @file mirror/auxiliary/counter.hpp
 *  @brief Helper template class for creating compile-time counters
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_COUNTER_1011291729_HPP
#define MIRROR_AUX_COUNTER_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/preprocessor.hpp>
#include <type_traits>

#ifndef MIRROR_COUNTER_LUID
#ifdef __COUNTER__
#define MIRROR_COUNTER_LUID __COUNTER__
#else
#define MIRROR_COUNTER_LUID __LINE__
#define MIRROR_COUNTER_UNRELIABLE
#endif
#endif

MIRROR_NAMESPACE_BEGIN

/// This namespace contains everything related to compile time counters
namespace cntr {

/** @defgroup ct_counters Mirror - Compile-time counters
 *
 *  This module contains a set of macros for declaring, initializing,
 *  incrementing and getting the current and the previews value
 *  of compile-time counters.
 */

// local null-type
struct null_ { };

// the initial value of the counter
typedef std::integral_constant<int, -1> initial;

// the zero value
typedef std::integral_constant<int,  0> zero;

// a fallback version of the get_next function
template <class Selector>
null_ get_next(Selector, ...);

// the get_next overload for the first incrementation
template <class Selector>
zero get_next(Selector, initial);

// forward declaration of the current counter template
template <class Selector, class Value, int InstId>
struct curr;

// helper template returning the next counter value for the Selector
template <
	class Selector,
	class Current,
	class Next,
	int InstId
>
struct nextval
{
	typedef typename curr<
		Selector,
		Next,
		InstId
	>::type type;
};

// Specialization of nextval for the current value
template <class Selector, class Current, int InstId>
struct nextval<Selector, Current, null_, InstId>
{
	typedef Current type;
};

// Returns the current value of the counter with the given Selector
template <class Selector, class Value, int InstId>
struct curr : nextval<
	Selector,
	Value,
	decltype(get_next(Selector(), Value())),
	InstId
>::type
{ };

// Forward declaration of the selector template
template <class T> struct sel;

} // namespace cntr
MIRROR_NAMESPACE_END

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Registers a new compile-time counter identified by the SELECTOR type
/** This macro registers a selector for the given (fully qualified)
 *  type.
 *
 *  The initial value of the counter is zero.
 *  This macro must be used between the @c MIRROR_REG_BEGIN and
 *  the @c MIRROR_REG_END macros.
 *
 *  @param SELECTOR name of a type which acts as the identifier of the counter
 *  @ingroup ct_counters
 */
#define MIRROR_REGISTER_COUNTER_SELECTOR(SELECTOR)
#else
#define MIRROR_REGISTER_COUNTER_SELECTOR(SELECTOR) \
namespace cntr { \
template <> struct sel < SELECTOR > { }; \
} /* namespace cntr */
#endif

// Helper macro used in implementation of MIRROR_COUNTER_CURRENT
#define MIRROR_COUNTER_CURRENT_COUNT(SELECTOR, LUID) \
	mirror::cntr::curr< \
		mirror::cntr::sel< SELECTOR >, \
		mirror::cntr::initial, \
		LUID \
	>::type

// Helper macro used for getting the previous value of a counter
#define MIRROR_COUNTER_PREVIOUS_COUNT_BASE(SELECTOR, LUID, TYPENAME_KW) \
	std::integral_constant< \
		int, \
		MIRROR_COUNTER_CURRENT_COUNT(SELECTOR, LUID)::value - 1 \
	>

// Helper macro used for implementation of MIRROR_COUNTER_PREVIOUS
#define MIRROR_COUNTER_PREVIOUS_COUNT(SELECTOR, LUID) \
	MIRROR_COUNTER_PREVIOUS_COUNT_BASE( \
		SELECTOR, \
		LUID, \
		MIRROR_PP_EMPTY() \
	)


// Helper macro used in implementation of MIRROR_COUNTER_INCREMENT
#define MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, LUID) \
namespace cntr { \
	std::integral_constant< \
		int, \
		curr< sel< SELECTOR >, initial, LUID >::value + 1 \
	> get_next( \
		sel< SELECTOR >, \
		curr< sel< SELECTOR >, initial, LUID >::type \
	); \
} /* namespace cntr */

/// Returns the value of a compile-time counter identified by the SELECTOR
/** This macro expands into a compile-time constant type representing the value
 *  of a counter identified by the SELECTOR type.
 *  The resulting type has the same interface as a standard compile-time
 *  std::integral_constant type.
 *
 *  This macro must be used between the @c MIRROR_REG_BEGIN and
 *  the @c MIRROR_REG_END macros.
 *
 *  @param SELECTOR name of a type which acts as the identifier of the counter
 *  @ingroup ct_counters
 */
#define MIRROR_COUNTER_CURRENT(SELECTOR) \
	MIRROR_COUNTER_CURRENT_COUNT( \
		SELECTOR, \
		MIRROR_COUNTER_LUID \
	)

/// Returns the previous value of a compile-time counter identified by SELECTOR
/** This macro expands into a compile-time constant type representing the
 *  previous value of a counter identified by the SELECTOR type.
 *  The resulting type has the same interface as a standard compile-time
 *  std::integral_constant type.
 *
 *  This macro must be used between the @c MIRROR_REG_BEGIN and
 *  the @c MIRROR_REG_END macros.
 *
 *  @param SELECTOR name of a type which acts as the identifier of the counter
 *  @ingroup ct_counters
 */
#define MIRROR_COUNTER_PREVIOUS(SELECTOR) \
	MIRROR_COUNTER_PREVIOUS_COUNT( \
		SELECTOR, \
		MIRROR_COUNTER_LUID \
	)

/// Increments the value of a compile-time counter identified by the SELECTOR
/** This macro increments the value of a counter identified by the SELECTOR
 *  type.
 *
 *  After the use of this macro in the source the value of the counter is
 *  incremented by one.
 *  This macro must be used between the @c MIRROR_REG_BEGIN and
 *  the @c MIRROR_REG_END macros.
 *
 *  @param SELECTOR name of a type which acts as the identifier of the counter
 *  @ingroup ct_counters
 */
#define MIRROR_COUNTER_INCREMENT(SELECTOR) \
	MIRROR_COUNTER_INCREMENT_COUNTER(SELECTOR, MIRROR_COUNTER_LUID)

#endif //include guard

