/**
 * @file mirror/invoker.hpp
 * @brief Public interface for function invokers
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_INVOKER_1011291729_HPP
#define MIRROR_INVOKER_1011291729_HPP

#include <mirror/auxiliary/func_call.hpp>

MIRROR_NAMESPACE_BEGIN

/** @defgroup invoker_generator Mirror - Invoker generator utility
 *
 */

/// Class template for generating function invokers
/** This template can be used to create a custom invoker class
 *  that uses the registered meta-data about a function and
 *  two user-specified templates create and pass the arguments
 *  to a function.
 *
 *  @tparam Manufacturer a class template which knows how to construct
 *  the parametes for the constructors. this template can for example
 *  use this factory template recursivelly for elaborated types and specify
 *  how to construct and initialize instances of the native C++ types.
 *
 *  @tparam Suppliers a class template which knows how to get existing
 *  instances to be supplied as parameters to the constructors.
 *
 *  @tparam Enumerator a class template which knows how to get enumerated
 *  values from external representations, like their names or values.
 *
 *  @tparam SourceTraits A parameter for the @c Manufacturer and @c Suppliers
 *  templates, which can be used for further configuration and fine-tuning
 *  of the generated factory.
 *
 *  @tparam Product the type instances of which are produced by this factory
 *
 *  @ingroup invoker_generator
 *
 *  @see factory_generator
 *  @see factory_maker
 */
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename MetaFunction
> class invoker : public aux::base_fn_caller<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	MetaFunction,
	typename parameters<MetaFunction>::type,
	typename mp::make_index_seq<
		typename mp::size<
			typename parameters<MetaFunction>::type
		>::type
	>::type
>
{
private:
	typedef typename aux::base_fn_caller<
		Manufacturer,
		Suppliers,
		Enumerator,
		SourceTraits,
		MetaFunction,
		typename parameters<MetaFunction>::type,
		typename mp::make_index_seq<
			typename mp::size<
				typename parameters<MetaFunction>::type
			>::type
		>::type
	> base_class;
public:
	template <typename Param>
	invoker(Param param)
	 : base_class(param)
	{ }
};


/// Helper class for more convenient instantiation of the invoker template
/** This template class can be used for a convenient creation of invokers
 *  of the same kind but calling different functions.
 *  If one wants to use the same kind of invoker (i.e. using the same
 *  @c Manufacturer, @c Suppliers and @c Enumerator templates and the same
 *  @c SourceTraits parameter) to call different functions, this
 *  template can be used in the following way:
 *  @code
 *  // implement the class which will be passed as the SourceTraits parameter
 *  class my_factory_param
 *  { ... some implementation ... };
 *
 *  // implement the parameter manufacturer template
 *  template <class Product, class Param>
 *  class my_manufacturer
 *  { ... some implementation ... };
 *
 *  // implement the parameter supplier(s) template
 *  template <class Product, class Param>
 *  class my_suppliers
 *  { ... some implementation ... };
 *
 *  // use the invoker_maker template to define a custom invoker maker
 *  typedef invoker_maker<
 *      my_manufacturer,
 *      my_suppliers,
 *      my_factory_param
 *  > my_maker;
 *
 *  // use the maker to create some factories
 *  my_maker::invoker<my_function_1>::type my_fn_1_invoker;
 *  my_maker::invoker<my_function_2>::type my_fn_2_invoker;
 *  my_maker::invoker<my_function_3>::type my_fn_3_invoker;
 *
 *  my_class x;
 *  my_fn_1_invoker.call_on(x);
 *  my_fn_2_invoker.call_on(x);
 *  my_fn_3_invoker.call_on(x);
 *
 *  @endcode
 *
 *  @tparam Manufacturer a class template which knows how to construct
 *  the parametes for the constructors. this template can for example
 *  use this factory template recursivelly for elaborated types and specify
 *  how to construct and initialize instances of the native C++ types.
 *
 *  @tparam Suppliers a class template which knows how to get existing
 *  instances to be supplied as parameters to the constructors.
 *
 *  @tparam Enumerator a class template which knows how to get enumerated
 *  values from external representations, like their names or values.
 *
 *  @tparam SourceTraits a parameter for the @c Manufacturer and @c Suppliers
 *  templates, which can be used for further configuration and fine-tuning
 *  of the generated factory.
 *
 *  @ingroup invoker_generator
 *
 *  @see invoker
 */
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits
> struct invoker_maker
{
	/// This meta-function creates an invoker creating the @c Product
	/** This meta-function returns a invoker class which can call
	 *  the function reflected by @c MetaFunction.
	 *
	 *  @tparam MetaFunction the meta-function reflecting the real function
	 *  to be called.
	 */
	template <typename MetaFunction>
	struct invoker
	{
		typedef mirror::invoker<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			MetaFunction
		> type;
	};
};

MIRROR_NAMESPACE_END

#endif //include guard

