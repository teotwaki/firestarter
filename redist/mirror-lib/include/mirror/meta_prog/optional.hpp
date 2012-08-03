/**
 * @file mirror/meta_prog/optional.hpp
 * @brief Compile-time optional type
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_OPTIONAL_1104130956_HPP
#define MIRROR_META_PROG_OPTIONAL_1104130956_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/nil_type.hpp>
#include <mirror/meta_prog/traits.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

/// A single value container
/**
 *  @tparam T the element of the optional
 *  @see nil
 *  @see get
 *  @ingroup meta_programming
 */
template <typename T>
struct optional
{
	typedef optional type;
};

/// Convenience typedef for NIL optional
typedef optional<nil_t> nil_optional;

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Nil intrinsic meta-function for optionals
/**
 *  @tparam Optional the optional to be examined
 *  @see optional
 *  @see get
 *  @ingroup meta_programming
 */
template <typename Optional>
struct nil { };
#else
// The default implementation of the empty meta-function
template <typename X>
struct nil : nil<typename X::type>::type
{
	MIRROR_ASSERT_RETURNS_OPTIONAL(X);
};
#endif

template <>
struct nil<nil_optional>
 : std::true_type
{ };

template <typename T>
struct nil<optional<T> >
 : std::false_type
{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the element of the optional passed as argument
/**
 *  This operation may be invoked only on non-nil optionals.
 *
 *  @tparam Optional the optional the item of which is to be returned
 *  @see optional
 *  @see nil
 *  @ingroup meta_programming
 */
template <typename Optional>
struct get
{
	/// The type in the optional passed as argument
	typedef unspecified_type type;
};
#else
template <typename X>
struct get
{
	MIRROR_ASSERT_RETURNS_OPTIONAL(X);
	typedef typename get<typename X::type>::type type;
};
#endif

template <typename T>
struct get<optional<T> >
{
	typedef T type;
};

namespace aux {

template <typename Expr, typename IsOptional>
struct as_optional;

template <typename Optional>
struct as_optional<Optional, std::true_type>
{
	typedef Optional type;
};

template <typename Expr>
struct as_optional<Expr, std::false_type>
{
	MIRROR_ASSERT_RETURNS_OPTIONAL(Expr);
	typedef typename Expr::type type;
};

} // namespace aux

template <typename Expr>
struct as_optional : public aux::as_optional<
	Expr,
	typename is_optional<Expr>::type
>{ };


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

