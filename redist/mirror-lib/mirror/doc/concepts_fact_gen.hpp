/**
 * @file mirror/doc/concepts_fact_gen.hpp
 * @brief Concepts defined by the factory generator utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_FACT_GEN_1011291729_HPP
#define MIRROR_DOC_CONCEPTS_FACT_GEN_1011291729_HPP

#include <mirror/config.hpp>

// This section is here for documentation purposes only
#ifdef MIRROR_DOCUMENTATION_ONLY

/** @defgroup factory_generator_concepts Mirror - Factory generator concepts
 *
 *  These classes are models of concepts defined by Mirror's factory
 *  generator utility.
 */

MIRROR_NAMESPACE_BEGIN

/// Contextual information for the argument sources of a generated factory
/** Models of the ConstructionInfo provide information during the initialization
 *  of the factory argument sources.
 *
 *  @ingroup factory_generator_concepts
 */
struct ConstructionInfo
{
	/// The context in which the factory argument source is used
	typedef Range<MetaNamedScopedObject> context;

	/// The function or constructor for which the argument is provided
	typedef MetaFunction function;

	/// The list of parameters of the constructor or function
	typedef Range<MetaParameter> parameters;

	/// The index of the parameter value of which is being provided
	typedef IntegralConstant parameter_index;

	/// The parameter value of which is being provided
	typedef MetaParameter parameter;
};

/// Contextual information for the constructor wrapper of a generated factory
/** Models of the ConstructorInfo provide information during the initialization
 *  of the factory constructor wrappers. Instances of ConstructorInfo are passed
 *  by the factory generator to the add_constructor(...) member function.
 *
 *  @ingroup factory_generator_concepts
 */
struct ConstructorInfo
{
	/// The context in which the factory argument source is used
	typedef Range<MetaNamedScopedObject> context;

	/// Meta class reflecting the product whose constructor is wrapped
	typedef MetaClass product;

	/// The index of the constructor that is being wrapped
	typedef IntegralConstant constructor_index;

	/// The constructor which is wrapped
	typedef MetaConstructor constructor;

	/// Constructor enumeration information
	EnumerationInfo enumeration_info;
};

/// Information about the direction of enumeration of constructors for Manager
/** Based on the implementation and the compiler used, the enumeration
 *  of constructors for a @c Manager may be either forward or backward.
 *  The responsibility of this structure is to provide information about
 *  the direction of the enumeration for the @c Manager.
 *
 *  @ingroup factory_generator_concepts
 */
struct EnumerationInfo
{
	/// The possible iteration direction
	enum direction_type {unknown, forward, backward};

	/// Returns the iteration direction
	direction_type direction(void) const;

	/// Returns true if iterating forward (0, 1, ..., N-1)
	bool going_backward(void) const;

	/// Returns true if iterating backward (N-1, ..., 1, 0)
	bool going_forward(void) const;
};

/// Suppliers are responsible for getting existing values to be copied from
/** Instantiations of this template provide existing values of a @c Product
 *  type that are used by copy constructors to create a new instance of
 *  @c Product in a generated factory.
 *
 *  One instance of Suppliers for a particular @c Product type having
 *  a copy constructor is created every time a factory needs to create
 *  an instance of @c Product either as the final product of construction
 *  or as an parameter for another constructor.
 *
 *  @ingroup factory_generator_concepts
 */
template <typename Product, typename SourceTraits>
struct Suppliers
{
	/// Constructor
	Suppliers(
		UnspecifiedType parent_data,
		ConstructionInfo construction_info
	);

	/// Post construction finishing of the Suppliers
	/** Some factories may need additional post-construction finishing
	 *  when the sources for all parameters are constructed, before the
	 *  Suppliers (and Manufacturers) are used. Such finishing can be
	 *  done here.
	 */
	void finish(UnspecifiedType parent_data);

	/// Function call operator returning an existing instance of @c Product
	Product operator()(void);
};

/// Manufacturer is responsible for the conversion of arguments for a constructor
/** Manufacturers are used to provide parameters for constructors in a factory.
 *  Unlike the Suppliers a manufacturer creates a new instance from an external
 *  data representation or in case of elaborated @c Product types it can use
 *  a @c factory<Manufacturer, Suppliers, SourceTraits, ...> recursivelly to
 *  create the required argument.
 *
 *  @ingroup factory_generator_concepts
 */
template <typename Product, typename SourceTraits>
struct Manufacturer
{
	/// Constructor
	Suppliers(
		UnspecifiedType parent_data,
		ConstructionInfo construction_info
	);

	/// Post construction finishing of the Manufacturer
	/** Some factories may need additional post-construction finishing
	 *  when the sources for all parameters are constructed, before the
	 *  Manufacturer (and Suppliers) are used. Such finishing can be
	 *  done here.
	 */
	void finish(UnspecifiedType parent_data);

	/// Function call operator returning a new instance of @c Product
	Product operator()(void);
};

/// Manager is reponsible for the selection of the constructor to be used
/** This overload of the Manufacturer template is also call the @c Manager
 *  since its role is to keep track of the available constructors and
 *  to pick one of them based on the input data or on user preference.
 *  @c Manager has therfore a different interface than a regular Manufacturer.
 *
 *  @ingroup factory_generator_concepts
 */
template <typename SourceTraits>
struct Manufacturer<void, SourceTraits>
{
	/// Constructor
	Suppliers(
		UnspecifiedType parent_data,
		ConstructionInfo construction_info
	);

	/// Post construction finishing of the Manager
	/** Some factories may need additional post-construction finishing
	 *  when the sources for all parameters and the Manager is created.
	 *  Such post-construction finishing can be done here.
	 */
	void finish(UnspecifiedType parent_data);

	/// This gets called once per every manager constructor of @c Product
	/** The factory calls this member function once per every constructor
	 *  of the @c Product type. The manager is responsible for storing
	 *  necessary meta-data about the constructor in order to be able to
	 *  pick one later when the @c index() member function is called.
	 *
	 *  The return value is data of factory-plugin-specific type that will
	 *  be passed as an argument to nested Manufacturer or Suppliers.
	 *
	 *  The constructors can be enumerated either forward or backward
	 *  the enum_info argument can provide hints about the direction
	 *  of the traversal or enumeration.
	 */
	UnspecifiedType add_constructor(
		UnspecifiedType parent_data,
		ConstructorInfo constructor_info
	);

	/// Returns the index of the constructor to be used
	int index(void);
};

MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

