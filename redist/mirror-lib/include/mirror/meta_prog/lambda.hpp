/**
 * @file mirror/meta_prog/lambda.hpp
 * @brief Lambda expressions
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_LAMBDA_1011291729_HPP
#define MIRROR_META_PROG_LAMBDA_1011291729_HPP


#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

/// Meta-function class returning the @a Number -th of its arguments
/** This template is used in two different ways. First it wraps a meta-function
 *  returning the N-th of its arguments. Second it is used as a placeholder
 *  for the N-th argument in placeholder lambda expressions.
 *
 *  @ingroup meta_programming
 */
template <unsigned Number>
struct arg
{
	template <typename ... Params>
	struct apply
	{
		typedef typename at_c<
			range<Params...>,
			Number - 1
		>::type type;
	};
};


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Invokes a meta-function-class or placeholder lambda expression.
/**
 *
 *  @tparam LambdaExpression the expression to be "called"
 *  @tparam Params the pack of params passed to the lambda expression
 *
 *  @ingroup meta_programming
 */
template <typename LambdaExpression, typename ... Params>
struct apply
{
	/// The result of the invocation of the expression with the params
	typedef unspecified type;
};
#else
// specialization for meta-function classes
template <typename MetaFunctionClass, typename ... Params>
struct apply
{
	typedef typename MetaFunctionClass::
		template apply<Params...>::type type;
};
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the N-th Param or the @a Expr expression
/** This template returns either one of the @a Params if the @a Expr
 *  type is an instantiation of the arg<N> template or the @a Expr
 *  type itself otherwise.
 *
 *  In the First case the N-th of the @a Params is returned where
 *  @a N is the argument to the @a arg placeholder.
 *
 *  @ingroup meta_programming
 */
template <typename Expr, typename ... Params>
struct use_arg
{
	/// Either the N-th argument or the Expr itself
	typedef unspecified type;
};
#else
// The default implementation which returns the Arg type
template <typename Arg, typename ... Params>
struct use_arg
{
	typedef Arg type;
};
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Protects the @a PlaceholderExpression from being expanded
/** This meta-function can be used to protect nested placeholder
 *  expressions from immediate expansion.
 *
 *  @ingroup meta_programming
 */
template <typename PlaceholderExpression>
struct protect
{
	typedef unspecified type;
};
#else
template <typename Expr>
struct protect
{
	typedef Expr type;
};
#endif

// Specialization for the arg<N> meta-function class
template <unsigned Number, typename ... Params>
struct use_arg<arg<Number>, Params...>
{
	typedef typename at_c<range<Params...>, Number - 1>::type type;
};

// Specialization for the arg<N> meta-function wrapped in 1 expression template
template <
	template <class> class Expr,
	unsigned Number,
	typename ... Params
>
struct use_arg<Expr<arg<Number> >, Params...>
{
	// get the N-th parameter and pass it to the expression template
	typedef typename Expr<
		typename at_c<range<Params...>, Number - 1>::type
	>::type type;
};

template <
	typename Expr,
	typename ... Params
>
struct use_arg<protect<Expr>, Params...>
{
	typedef Expr type;
};

template <
	template <class...> class Expr,
	typename ... SubExprs,
	typename ... Params
>
struct use_arg<Expr<SubExprs...>, Params...>
{
	typedef typename Expr<
		typename use_arg<
			SubExprs,
			Params...
		>::type...
	>::type type;
};

// specialization for expression template instantiations
template <
	template <typename ...> class Expr,
	typename ... ExprArgs,
	typename ... Params
>
struct apply<Expr<ExprArgs...>, Params...>
{
	typedef typename Expr<
		typename use_arg<ExprArgs, Params...>::type ...
	>::type type;
};

// specialization for the arg<N> meta-function class
template <unsigned Number, typename ... Params>
struct apply<arg<Number>, Params...>
{
	typedef typename arg<Number>::
		template apply<Params...>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

