/**
 * @file mirror/factory.hpp
 * @brief Public interface for factory generators
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_FACTORY_1011291729_HPP
#define MIRROR_FACTORY_1011291729_HPP

#include <mirror/auxiliary/factory.hpp>
#include <mirror/utils/native_types.hpp>
#include <mirror/meta_class.hpp>
#include <mirror/meta_enum.hpp>

MIRROR_NAMESPACE_BEGIN

/** @defgroup factory_generator Mirror - Factory generator utility
 *
 *  The factory generator utility allows to easily create implementations
 *  of object factories. Factories are objects which can create
 *  instances of various types (@c Products) by gathering
 *  the parameters necessary for the construction in
 *  an application-defined way, they pick or let the user pick
 *  the most appropriate constructor and use the @c Product 's constructor
 *  to create the instance.
 *  More on the principles and the usage of the factory generator can be found
 *  @link mirror_factory_generator_utility here@endlink.
 */

template <typename T>
struct is_specialized_factory_src;

template <
	typename Product,
	typename Traits,
	template <class, class> class Manufacturer
> struct is_specialized_factory_src<Manufacturer<Product, Traits> >
{
	typedef std::integral_constant<
		bool,
		is_util_native_type<Product>::value ||
		std::is_enum<Product>::value
	> type;
};

/// Class template for generation of object factories
/** This template can be used to create a custom factory class
 *  that uses the registered meta-data about constructors and
 *  two user-specified templates to select which of the constructors
 *  to use to create an instance and to supply the necessary parameters
 *  if necessary.
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
 *  @ingroup factory_generator
 *
 *  @see factory_maker
 */
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product
> class factory : public aux::base_factory<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Product,
	typename constructors< meta_class<Product> >::type,
	typename mp::make_index_seq<
		typename mp::size<
			constructors< meta_class<Product> >
		>::type
	>::type,
	typename is_specialized_factory_src<
		Manufacturer<Product, SourceTraits>
	>::type
>
{
private:
	typedef typename aux::base_factory<
		Manufacturer,
		Suppliers,
		Enumerator,
		SourceTraits,
		Product,
		typename constructors< meta_class<Product> >::type,
		typename mp::make_index_seq<
			typename mp::size<
				constructors< meta_class<Product> >
			>::type
		>::type,
		typename is_specialized_factory_src<
			Manufacturer<Product, SourceTraits>
		>::type
	> base_class;
public:
	template <typename Param>
	factory(Param param)
	 : base_class(param, mp::empty_range())
	{ }

	template <typename Param, class ConstructionInfo>
	factory(Param param, ConstructionInfo)
	 : base_class(param, typename ConstructionInfo::context())
	{ }

	template <typename Param, class ... Items>
	factory(Param param, mp::range<Items...> context)
	 : base_class(param, context)
	{ }
};


/// Helper class for more convenient instantiation of the factory template
/** This template class can be used for a convenient creation of factories
 *  of the same kind but generating different types.
 *  If one wants to use the same kind of factory (i.e. using the same
 *  @c Manufacturer, @c Suppliers and @c Enumerator templates and the same
 *  @c SourceTraits parameter) to create different products, this
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
 *  // implement the parameter enumerator template
 *  template <class Product, class Param>
 *  class my_enumerator
 *  { ... some implementation ... };
 *
 *  // use the factory_maker template to define a custom factory maker
 *  typedef factory_maker<
 *      my_manufacturer,
 *      my_suppliers,
 *      my_enumerator,
 *      my_factory_param
 *  > my_maker;
 *
 *  // use the maker to create some factories
 *  my_maker::factory<my_type_1>::type my_type_1_factory;
 *  my_maker::factory<my_type_2>::type my_type_2_factory;
 *  my_maker::factory<my_type_2>::type my_type_3_factory;
 *
 *  my_type_1* x1 = my_type_1_factory.new_();
 *  my_type_2* x2 = my_type_2_factory.new_();
 *  my_type_3* x3 = my_type_3_factory.new_();
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
 *  @tparam SourceTraits a parameter for the @c Manufacturer, @c Suppliers
 *  and @c Enumerator templates, which can be used for further configuration
 *  and fine-tuning of the generated factory.
 *
 *  @ingroup factory_generator
 *
 *  @see factory
 */
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits
> struct factory_maker
{
	/// This meta-function creates a factory creating the @c Product
	/** This meta-function returns a factory class which can create
	 *  instances of the @c Product type.
	 *
	 *  @tparam Product the type instances of which are to be created
	 *  by the resulting factory.
	 */
	template <typename Product>
	struct factory
	{
		typedef mirror::factory<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			Product
		> type;
	};
};

MIRROR_NAMESPACE_END

#endif //include guard

