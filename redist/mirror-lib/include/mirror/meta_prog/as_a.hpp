/**
 * @file mirror/meta_prog/as_a.hpp
 * @brief Evaluating of a meta-object expression to a specific concept
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_AS_A_1011291729_HPP
#define MIRROR_META_PROG_AS_A_1011291729_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/not.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace mp {
namespace aux {

template <class X, class RealConcept, class QueriedConcept, class True>
struct as_a_getter
{
	typedef X type;
};

template <class X, class RealConcept, class QueriedConcept>
struct as_a_helper
 : public as_a_getter<
	X, RealConcept, QueriedConcept,
	typename ::std::is_base_of<QueriedConcept, RealConcept>::type
>
{ };

template <class X, class Concept>
struct as_a_helper<X, Concept, Concept>
{
	typedef X type;
};

template <class X, class QueriedConcept>
struct as_a_helper<X, non_meta_object_tag, QueriedConcept>
 : public as_a_getter<X, non_meta_object_tag, QueriedConcept, std::false_type>
{ };

template <class X, class RealConcept, class QueriedConcept>
struct as_a_getter<X, RealConcept, QueriedConcept, std::false_type>
 : public as_a_helper<
	typename X::type,
	typename meta_object_category<typename X::type>::type,
	QueriedConcept
>{ };

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Tries to evaluate the MetaObjectExpression as a specific Concept type
/**
 *  @tparam MetaObjectExpression
 *  @tparam Concept
 *
 *  @ingroup meta_programming
 */
template <class MetaObjectExpr, class Concept>
struct as_a
{
	/// MetaObjectExpression "cast" to the Concept
	typedef unspecified type;
};
#else
template <class X, class Concept>
struct as_a : aux::as_a_helper<
	X,
	typename meta_object_category<X>::type,
	Concept
>
{ };
#endif

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

