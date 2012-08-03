/**
 * @file mirror/ct_string/default.hpp
 * @brief Default implementation of the compile-time strings algorithms
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_DEFAULT_1105240825_HPP
#define MIRROR_CT_STRING_DEFAULT_1105240825_HPP

#include <mirror/config.hpp>
#include <mirror/ct_string/basic_string.hpp>
#include <mirror/ct_string/evaluate.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

/// Meta-function returning the character type of a compile-time string
/**
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct char_type
#ifndef MIRROR_DOCUMENTATION_ONLY
 : char_type<typename evaluate<CTString>::type>
{ };
#else
{
	/// The character type use by the examined compile-time string
	typedef unspecified_char_type type;
};
#endif

/// Meta-function returning true_type if a compile-time string is empty
/**
 *  @see basic_string
 *  @see nonempty
 *  @see size
 *  @see length
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct empty
#ifndef MIRROR_DOCUMENTATION_ONLY
 : empty<typename evaluate<CTString>::type>
#else
 : BooleanConstantType
#endif
{ };

/// Meta-function returning true_type if a compile-time string is not empty
/**
 *  @see basic_string
 *  @see empty
 *  @see size
 *  @see length
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct nonempty
#ifndef MIRROR_DOCUMENTATION_ONLY
 : nonempty<typename evaluate<CTString>::type>
#else
 : BooleanConstantType
#endif
{ };

/// Meta-function returning the length of a compile-time string
/**
 *  This meta-function is equivalent to the @c length meta-function.
 *  @see basic_string
 *  @see empty
 *  @see nonempty
 *  @see length
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct size
#ifndef MIRROR_DOCUMENTATION_ONLY
 : size<typename evaluate<CTString>::type>
#else
 : IntegralConstantType
#endif
{ };

/// Meta-function returning the length of a compile-time string
/**
 *  This meta-function is equivalent to the @c size meta-function.
 *  @see basic_string
 *  @see empty
 *  @see nonempty
 *  @see size
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct length
#ifndef MIRROR_DOCUMENTATION_ONLY
 : length<typename evaluate<CTString>::type>
#else
 : IntegralConstantType
#endif
{ };

/// Equality comparison meta-function for compile-time strings
/**
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct equal
#ifndef MIRROR_DOCUMENTATION_ONLY
 : equal<
	typename evaluate<CTString1>::type,
	typename evaluate<CTString2>::type
>
#else
 : BooleanConstantType
#endif
{ };

/// Non-equality comparison meta-function for compile-time strings
/**
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct nonequal
#ifndef MIRROR_DOCUMENTATION_ONLY
 : nonequal<
	typename evaluate<CTString1>::type,
	typename evaluate<CTString2>::type
>
#else
 : BooleanConstantType
#endif
{ };

/// Meta-function returning the first character in the compile-time string
/**
 *  @see basic_string
 *  @see empty
 *  @see nonempty
 *  @see step_front
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct front
#ifndef MIRROR_DOCUMENTATION_ONLY
 : front<typename evaluate<CTString>::type>
#else
 : CharacterConstantType
#endif
{ };

/// Meta-function returning a compile-string without the first character
/**
 *  @see basic_string
 *  @see empty
 *  @see nonempty
 *  @see front
 *  @see skip_front
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct step_front
#ifndef MIRROR_DOCUMENTATION_ONLY
 : step_front<typename evaluate<CTString>::type>
{ };
#else
{
	/// The original compile-time string without the first character
	typedef CompileTimeString type;
};
#endif

/// Meta-function concatenating several compile-time strings together
/**
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename ... CTStrings>
struct concat
#ifndef MIRROR_DOCUMENTATION_ONLY
 : concat<typename evaluate<CTStrings>::type...>
{ };
#else
{
	/// The concatenated compile-time string
	typedef CompileTimeString type;
};
#endif

/// Meta-function returning a string containing the first N characters
/**
 *  @see basic_string
 *  @see tail
 *  @see slice
 *
 *  @ingroup ct_string
 */
template <typename CTString, typename Length>
struct head
#ifndef MIRROR_DOCUMENTATION_ONLY
 : head<
	typename evaluate<CTString>::type,
	typename Length::type
> { };
#else
{
	/// The head of the original compile-time string
	typedef CompileTimeString type;
};
#endif

/// Meta-function returning a string containing the first N characters
/**
 *  @see basic_string
 *  @see head_c
 *  @see tail
 *  @see slice
 *
 *  @ingroup ct_string
 */
template <typename CTString, int Length>
struct head_c;

/// Meta-function returning a string containing the last N characters
/**
 *  @see basic_string
 *  @see head
 *  @see tail
 *  @see tail_c
 *  @see slice
 *
 *  @ingroup ct_string
 */
template <typename CTString, typename Length>
struct tail
#ifndef MIRROR_DOCUMENTATION_ONLY
 : tail<
	typename evaluate<CTString>::type,
	typename Length::type
> { };
#else
{
	/// The tail of the original compile-time string
	typedef CompileTimeString type;
};
#endif

/// Meta-function returning a string containing the last N characters
/**
 *  @see basic_string
 *  @see head
 *  @see tail
 *  @see slice
 *
 *  @ingroup ct_string
 */
template <typename CTString, int Length>
struct tail_c;

/// Meta-function returning a string starting with the searched sub-string
/** This meta-function template returns a substring of the original
 *  compile-time string, that starts with the searched string (in case
 *  the original contains it) or an empty string (otherwise).
 *
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct find
#ifndef MIRROR_DOCUMENTATION_ONLY
 : find<
	typename evaluate<CTString1>::type,
	typename evaluate<CTString2>::type
> { };
#else
{
	/// Sub-string starting with the searched string or empty string
	typedef CompileTimeString type;
};
#endif

/// Meta-function returning a sub-string of a compile-time string
/**
 *  @see basic_string
 *  @see head
 *  @see tail
 *
 *  @ingroup ct_string
 */
template <typename CTString, typename Start, typename Length>
struct slice
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
{
	/// Sub-string starting at a specified position with the given length
	typedef CompileTimeString type;
};
#endif

/// Meta-function returning a sub-string of a compile-time string
/**
 *  @see basic_string
 *  @see head_c
 *  @see tail_c
 *
 *  @ingroup ct_string
 */
template <typename CTString, int Start, int Length>
struct slice_c;

/// Meta-function returning a sub-string without the first N characters
/**
 *  @see basic_string
 *  @see head
 *  @see tail
 *  @see step_front
 *
 *  @ingroup ct_string
 */
template <typename CTString, typename Length>
struct skip_front
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
{
	/// Sub-string of the original string starting at the specified position
	typedef CompileTimeString type;
};
#endif

/// Meta-function returning a sub-string without the first N characters
/**
 *  @see basic_string
 *  @see head_c
 *  @see tail_c
 *  @see step_front
 *
 *  @ingroup ct_string
 */
template <typename CTString, int Count>
struct skip_front_c;

/// Meta-function returning the character at the N-th position
/**
 *  @see basic_string
 *  @see head
 *  @see front
 *
 *  @ingroup ct_string
 */
template <typename CTString, typename Position>
struct at
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
 : CharacterConstantType
{ };
#endif

/// Meta-function returning the character at the N-th position
/**
 *  @see basic_string
 *  @see head_c
 *  @see front
 *
 *  @ingroup ct_string
 */
template <typename CTString, int Position>
struct at_c;

/// Meta-function returns true_type if the string starts with another string
/**
 *  @see basic_string
 *  @see head
 *  @see equal
 *  @see contains
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct starts_with
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
 : BooleanConstantType
{ };
#endif


/// Meta-function returns true_type if the string ends with another string
/**
 *  @see basic_string
 *  @see tail
 *  @see equal
 *  @see contains
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct ends_with
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
 : BooleanConstantType
{ };
#endif

/// Meta-function returns true_type if the string contains another string
/**
 *  @see basic_string
 *  @see equal
 *  @see lacks
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct contains
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
 : BooleanConstantType
{ };
#endif

/// Meta-function returns true_type if the string does not contain another string
/**
 *  @see basic_string
 *  @see equal
 *  @see contains
 *
 *  @ingroup ct_string
 */
template <typename CTString1, typename CTString2>
struct lacks
#ifndef MIRROR_DOCUMENTATION_ONLY
;
#else
 : BooleanConstantType
{ };
#endif

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

