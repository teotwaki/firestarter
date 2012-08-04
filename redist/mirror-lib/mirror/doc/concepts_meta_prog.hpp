/**
 *  @file mirror/doc/concepts_meta_prog.hpp
 *  @brief Documentation of meta-programming concepts
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_META_PROG_1104131331_HPP
#define MIRROR_DOC_CONCEPTS_META_PROG_1104131331_HPP

#include <mirror/config.hpp>

#ifdef MIRROR_DOCUMENTATION_ONLY

MIRROR_NAMESPACE_BEGIN
// This section is here for documentation purposes only

/** @defgroup meta_prog_concepts Mirror - Meta-programming concepts
 *
 *  These concept classes are defined here only for documentation purposes
 *  and cannot be used in any end-user code.
 */

/// Integral constant type
/**
 *  @em Models:
 *   - @c std::integral_constant<int, N>
 *
 *  @ingroup meta_prog_concepts
 */
struct IntegralConstantType
{
	static const int value;
	typedef IntegralConstantType type;
};

/// Boolean constant type
/**
 *  @em Models:
 *   - @c std::integral_constant<bool, B>
 *
 *  @ingroup meta_prog_concepts
 */
struct BooleanConstantType
{
	static const bool value;
	typedef BooleanConstantType type;
};

/// Element of Range or Optional
/**
 *  @ingroup meta_prog_concepts
 */
struct Element { };

/// Range is a typelist class containing a sequence of types
/** Ranges are usually returned by various intrinsic meta-functions which
 *  have a sequence of meta-objects as result.
 *
 *  @em Models:
 *   - @c mirror::mp::range
 *
 *  @see mirror::mp::range
 *  @see mirror::mp::empty
 *  @see mirror::mp::size
 *  @see mirror::mp::front
 *  @see mirror::mp::step_front
 *  @see mirror::mp::at
 *  @see mirror::mp::at_c
 *  @ingroup meta_prog_concepts
 */
template <typename ... Elements>
struct Range
{
	/// Meta-function checking if a Range is empty;
	/**
	 *  @see mirror::mp::empty
	 *  @returns BooleanConstantType
	 */
	friend struct empty<Range>;

	/// Meta-function returning the number of elements in a Range
	/**
	 *  @see mirror::mp::size
	 *  @returns IntegralConstantType
	 */
	friend struct size<Range>;

	/// Meta-function returning the front element of a Range
	/**
	 *  @see mirror::mp::front
	 *  @returns Element
	 */
	friend struct front<Range>;

	/// Meta-function returning a Range without the front element
	/**
	 *  @see mirror::mp::step_front
	 *  @returns Range
	 */
	friend struct step_front<Range>;

	/// Meta-function returning an element at the specified Position
	/**
	 *  @see mirror::mp::at
	 *  @returns Element
	 */
	friend struct at<Range, Position>;

	/// Meta-function returning an element at the specified Position
	/**
	 *  @see mirror::mp::at_c
	 *  @returns Element
	 */
	friend struct at_c<Range, Position>;
};

/// Optional is a class that can optionally contain a single type element
/**
 *  @em Models:
 *   - @c mirror::mp::optional
 *
 *  @see mirror::mp::optional
 *  @see mirror::mp::nil
 *  @see mirror::mp::get
 *  @ingroup meta_prog_concepts
 */
template <typename Element>
struct Optional
{
	/// Meta-function checking if a Optional is nil;
	/**
	 *  @see mirror::mp::nil
	 *  @returns BooleanConstantType
	 */
	friend struct nil<Optional>;

	/// Meta-function returning the number of elements in a Optional
	/**
	 *  @see mirror::mp::get
	 *  @returns Element
	 */
	friend struct get<Optional>;
};


MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

