/**
 * @file mirror/doc/concepts.hpp
 * @brief Classes modelling the concepts defined by Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_1011291729_HPP
#define MIRROR_DOC_CONCEPTS_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/doc/concepts_fwd.hpp>

// This section is here for documentation purposes only
#ifdef MIRROR_DOCUMENTATION_ONLY

/** @defgroup meta_object_concepts Mirror - Meta-object concepts
 *
 *  These classes are models of the concepts defined by Mirror. They are
 *  defined for documentation purposes only and cannot be used in any
 *  end-user code.
 */

MIRROR_NAMESPACE_BEGIN

/// MetaObject is a type defined by the Mirror library to provide meta-data
/**
 *
 *  @see meta_object_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaObject
{
	/// Meta-function for checking if the MetaObject conforms to a concept
	/** This meta-function returns std::true_type if a MetaObject conforms
	 *  to the concept identified by the @p ConceptTag.
	 *  Returns std::false_type otherwise.
	 *  @returns BooleanConstantType
	 */
	friend struct is_a<MetaObject, ConceptTag>;

	/** This function can be used to determine if a type is a MetaObject
	 *  @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_object_tag categorize_meta_object(MetaObject);
};

/// MetaNamedObject is a type reflecting a construct (type, ...) having a name
/** MetaNamedObject reflects base-level constructs which are identified
 *  by a name (identifier). Examples of named objects are types, namespaces,
 *  classes, functions, class members, etc. even if some of them might be
 *  anonymous.
 *  Meta objects reflecting these named objects provide functions for getting
 *  their names.
 *
 *  @see meta_named_object_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaNamedObject
 : virtual public MetaObject
{
	/// Returns the base name of the reflected entity
	/** This function returns the base name of the reflected
	 *  construct (type, class, namespace, member variable, etc.)
	 *  without the nested name specifier.
	 */
	static std::string base_name(void);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_named_object_tag categorize_meta_object(MetaNamedObject);
};

/// MetaMetaObject is a type reflecting a meta_object
/**
 *  @see meta_meta_object_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaObject
 *  @see MetaMetaObject
 *  @ingroup meta_object_concepts
 */
struct MetaMetaObject
 : virtual public MetaNamedObject
{
	/// Returns the name of the reflected language construct
	/** This function returns a human readable (i.e. not and
	 *  identifier) name of the language construct reflected
	 *  by the MetaObject which is reflected by this
	 *  MetaMetaObject.
	 */
	static std::string construct_name(void);

	/// Returns the range of generalizations for the reflected MetaObject
	/** This meta-function template returns a range of MetaMetaObjects
	 *  that reflect the generalizations of the MetaObject reflected
	 *  by this MetaMetaObject.
	 *
	 *  @returns Range<MetaMetaObject>
	 *
	 *  @see mp::range
	 */
	friend struct generalizations<MetaMetaObject>;

	/// The category of the reflected meta-object
	/**
	 *  This is the category of the meta-object reflected by this
	 *  meta-meta-object, same as returned by the meta_object_category
	 *  meta-function.
	 *
	 *  @see meta_object_category
	 *  @see categorize_meta_object
	 */
	typedef unspecified_type category;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_meta_object_tag
	categorize_meta_object(MetaMetaObject);
};


/// MetaScopedObject is a type reflecting a construct defined in a scope
/**
 *
 *  @see meta_scoped_object_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaScopedObject
 : virtual public MetaObject
{
	/// A type reflecting the scope of the scoped object
	/** This type conforms to the MetaScope concept
	 *  and provides meta-data about the scope which
	 *  encloses the construct reflected by this MetaScopedObject.
	 *  @see MetaScope
	 */
	typedef MetaScope scope;

	/// Meta-function returning the scope of the scoped object
	/** This meta-function "returns" the same MetaScope as the @c scope
	 *  member typedef, but is better suited for algorithms using
	 *  lambda placeholder expressions.
	 *  @returns MetaScope
	 */
	friend struct scope<MetaScopedObject>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_scoped_object_tag categorize_meta_object(MetaScopedObject);
};

/// MetaNamedScopedObject is a type reflecting a named construct defined in a scope
/**
 *
 *  @see meta_named_scoped_object_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaNamedScopedObject
 : virtual public MetaNamedObject
 , virtual public MetaScopedObject
{
	/// Returns the full name of the reflected entity
	/** This function returns the full name of the reflected
	 *  construct (type, class, namespace, member variable, etc.)
	 *  including the nested name specifier.
	 */
	static std::string full_name(void);

	/// Returns the local name of the reflected entity
	/** This function returns the local name of the reflected
	 *  construct (type, class, namespace, member variable, etc.)
	 *  including the partial or full nested name specifier, based
	 *  on whether the parent scope or one of its enclosing scopes
	 *  of the construct reflected by this MetaNamedScopedObject
	 *  is used with the MIRROR_USING_NAMESPACE macro.
	 *
	 *  @see MIRROR_USING_NAMESPACE
	 */
	static std::string local_name(void);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_named_scoped_object_tag
	categorize_meta_object(MetaNamedScopedObject);
};

/// MetaScope is a type reflecting a construct enclosing other constructs
/**
 *
 *  @see meta_scope_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaScope
 : virtual public MetaNamedScopedObject
{

	/** Meta-function returning a range of MetaScopedObjects
	 *  which are reflecting the individual members of the scope.
	 *  @see members
	 *  @see mp::range
	 *  @returns Range<MetaScopedObject>
	 */
	friend struct members<MetaScope>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_scope_tag categorize_meta_object(MetaScope);
};

/// MetaNamespace is a type reflecting namespaces
/**
 *
 *  @see meta_namespace_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaNamespace
 : virtual public MetaScope
{
	/// A type reflecting the scope of the namespace
	typedef MetaNamespace scope;

	/** Meta-function returning a range of MetaNamespaces
	 *  which are reflecting the nested namespaces of the reflected namespace.
	 *
	 *  @see mp::range
	 *  @returns Range<MetaNamespace>
	 */
	friend struct namespaces<MetaNamespace>;

	/** Meta-function returning a range of MetaFreeVariables
	 *  which are reflecting the free variables of the reflected namespace.
	 *
	 *  @see mp::range
	 *  @returns Range<MetaFreeVariable>
	 */
	friend struct free_variables<MetaNamespace>;

	/** Meta-function returning a range of MetaOverloadedFunctions
	 *
	 *  @see MetaOverloadedFunctions
	 *  @see mp::range
	 *  @returns Range<MetaOverloadedFunctions>
	 */
	friend struct free_functions<MetaNamespace>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_namespace_tag categorize_meta_object(MetaNamespace);
};

/// MetaGlobalScope is a type reflecting the global scope namespace
/**
 *
 *  @see meta_global_scope_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaGlobalScope
 : virtual public MetaNamespace
{
	/// A type reflecting the scope of the global scope
	typedef MetaGlobalScope scope;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_global_scope_tag categorize_meta_object(MetaNamespace);
};

/// MetaType is a type reflecting base-level types
/**
 *
 *  @see meta_type_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaType
 : virtual public MetaNamedScopedObject
{
	/// The base-level type reflected by this MetaType
	typedef unspecified_type original_type;

	/// Meta-function returning the type reflected by this MetaType.
	/** This meta-function "returns" the same type as the @c original_type
	 *  member typedef, but is better suited for algorithms using
	 *  lambda placeholder expressions.
	 */
	friend struct original_type<MetaType>;

	/** Meta-function returning MetaType reflecting a type that is the
	 *  result of application of the @a ModifierLambdaExpression on the
	 *  @c original_type reflected by the passed @c MetaType.
	 *
	 *  @tparam MetaType meta-type reflecting the original type to be
	 *  modified and reflected.
	 *
	 *  @tparam ModifierLambdaExpression unary lambda expression doing
	 *  the modification of the original type. For example:
	 *  @code
	 *  std::add_pointer<mirror::mp::arg<1>>
	 *  @endcode
	 *  The type trait templates used to construct the modifier expression
	 *  must not return a boolean or integral constants.
	 *
	 *  @returns MetaType
	 */
	friend struct apply_modifier<MetaType, ModifierLambdaExpression>;

	/** Meta-function returning a range of MetaConstructors
	 *  which are reflecting the constructors of the reflected class
	 *
	 *  @see mp::range
	 *  @returns Range<MetaConstructor>
	 */
	friend struct constructors<MetaClass>;

	/// Meta-function returning the container kind of a class
	/** This meta-function returns a container kind tag indicating if
	 *  and what kind of container the reflected class is.
	 *
	 *  @returns ContainerKindTag
	 */
	friend struct container_kind<MetaType>;

	/** Meta-function returning a range of MetaContainerss
	 *  which are reflecting the element containers of the reflected type
	 *
	 *  @see MetaClass::all_containers
	 *  @see mp::range
	 *  @returns Range<MetaContainer>
	 */
	friend struct containers<MetaType>;

	/// Convenience function for default element traversal
	/** This function calls the provided pair of functors
	 *  on each element in the instance of the container
	 *  type reflected by this MetaType or on each empty subcontainer.
	 *
	 *  The @c elem_func functor is called on each element in the
	 *  non-empty subcontainers and should accept three parameters:
	 *  the first is a tag type indicating the kind of the container
	 *  he second is an instance of the element_traversal_info
	 *  type containing information about the position of the currently
	 *  traversed element and the third is the currently traversed element.
	 *
	 *  The @c empty_func functor is called on the empty subcontainers
	 *  and should accept two parameters:
	 *  the first is a tag type indicating the kind of the container
	 *  and the second is an instance of the element_traversal_info
	 *  type containing information about the status of the traversal.
	 *
	 *  This function returns the number of elements traversed.
	 *
	 *  @see containers
	 *  @see MetaClass::all_containers
	 *  @see container_tag
	 *  @see element_traversal_info
	 */
	friend int traverse_elements(
		MetaType meta_type,
		MetaType::original_type& container,
		TernaryFunctor elem_func,
		BinaryFunctor empty_func
	);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_type_tag categorize_meta_object(MetaType);
};

/// MetaTypedef is a type reflecting base-level typedefined types
/**
 *  @see meta_typedef_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaTypedef
 : virtual public MetaType
{
	/// MetaType reflecting the base type used in the type definition
	typedef MetaType type;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_typedef_tag categorize_meta_object(MetaTypedef);
};

/// MetaTemplatedType is a type reflecting instantiation of a type template
/**
 *
 *  @see meta_templated_type_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaTemplatedType
 : virtual public MetaType
{
	/** The MetaTypeTemplate reflecting the template which
	 *  was used for the instantiation of this templated type.
	 *
	 *  @see parameters
	 *  @see MetaTypeTemplate
	 */
	typedef MetaTypeTemplate type_template;

	/// Meta-function returning the template of the templated type
	/** This meta-function "returns" the same MetaTypeTemplate as the
	 *  @c type_template member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns MetaTypeTemplate
	 */
	friend struct type_template<MetaTemplatedType>;

	/** Meta-function returning a range of MetaTypes
	 *  which are reflecting the individual parameters
	 *  used in the instantiation of the type template.
	 *  @see members
	 *  @see mp::range
	 *  @returns Range<MetaType>
	 */
	friend struct template_parameters<MetaTemplatedType>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_templated_type_tag
	categorize_meta_object(MetaTemplatedType);
};

/// MetaTypeTemplate is a type reflecting base-level type templates
/**
 *
 *  @see meta_type_template_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaTypeTemplate
 : virtual public MetaNamedScopedObject
{
	/// This meta-function for instantiation of the reflected template
	/** Meta-function template which instantiates the template
	 *  reflected by this MetaTypeTemplate with the given
	 *  paramters.
	 */
	template <typename ... P>
	struct reflected_template
	{
		/// The instantiation of the reflected template
		typedef unspecified_type type;
	};


	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_type_template_tag
	categorize_meta_object(MetaTypeTemplate);
};

/// MetaClass is a type reflecting base-level classes
/**
 *  @see meta_class_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaClass
 : virtual public MetaType
 , virtual public MetaScope
{
	/// The elaborated type specifier tag
	/** Tag reflecting the elaborate type specifier (class | struct |
	 *  union) that was specified during definition (and registering)
	 *  of the class reflected by this meta_class.
	 *
	 *  @see ElaboratedTypeTag
	 */
	typedef ElaboratedTypeTag elaborated_type;

	/// Meta-function returning the elaborated type specifier of a class
	/** This meta-function "returns" the same elaborated type specifier as
	 *  the @c elaborated_type member typedef, but is better suited
	 *  for algorithms using lambda placeholder expressions.
	 *  @returns ElaboratedTypeTag
	 */
	friend struct elaborated_type<MetaClass>;

	/** Meta-function returning a range of MetaInheritances
	 *  which are reflecting the inheritance of the base classes
	 *  @see base_classes
	 *  @see mp::range
	 *  @returns Range<MetaInheritance>
	 */
	friend struct base_classes<MetaClass>;

	/** Meta-function returning a range of MetaMemberVariables
	 *  which are reflecting the member variables of the reflected class
	 *  not including those inherited from base classes.
	 *
	 *  @see all_member_variables
	 *  @see mp::range
	 *  @returns Range<MetaMemberVariable>
	 */
	friend struct member_variables<MetaClass>;

	/** Meta-function returning a range of MetaMemberVariables
	 *  which are reflecting the member variables of the reflected class
	 *  including those inherited from base classes.
	 *
	 *  @see member_variables
	 *  @see mp::range
	 *  @returns Range<MetaMemberVariable>
	 */
	friend struct all_member_variables<MetaClass>;

	/** Meta-function returning a range of MetaOverloadedFunctions which
	 *  are reflecting the overloaded functions of the reflected
	 *  class, not including those inherited from the base classes.
	 *
	 *  @see all_member_functions
	 *  @see mp::range
	 *  returns Range<MetaOverloadedFunctions>
	 */
	friend struct member_functions<MetaClass>;

	/** Meta-function returning a range of MetaOverloadedFunctions which
	 *  are reflecting the overloaded functions of the reflected
	 *  class, including those inherited from the base classes.
	 *
	 *  @see member_functions
	 *  @see mp::range
	 *  returns Range<MetaOverloadedFunctions>
	 */
	friend struct all_member_functions<MetaClass>;


	/** Meta-function returning a range of MetaContainerss
	 *  which are reflecting the element containers of the reflected class
	 *  including those inherited from base classes.
	 *
	 *  @see MetaType::containers
	 *  @see mp::range
	 *  @returns Range<MetaContainer>
	 */
	friend struct all_containers<MetaClass>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_class_tag categorize_meta_object(MetaClass);
};

/// MetaClassMember is a type reflecting class member variables and functions
/**
 *  @see meta_class_member_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaClassMember
 : virtual public MetaNamedScopedObject
{
	/// The class in which the member is defined
	typedef MetaClass scope;

	/// Meta-function returning the scope of the class member
	/** This meta-function "returns" the same MetaScope as the @c scope
	 *  member typedef, but is better suited for algorithms using
	 *  lambda placeholder expressions.
	 *  @returns MetaClass
	 */
	friend struct scope<MetaClassMember>;

	/// The access type tag
	/** Tag reflecting the access type (private | protected | public)
	 *  for the reflected class member variable, function or constructor.
	 *
	 *  @see AccessTypeTag
	 */
	typedef AccessTypeTag access_type;

	/// Meta-function returning the access type of the class member
	/** This meta-function "returns" the same AccessTypeTag as
	 *  the @c access_type member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns AccessTypeTag
	 */
	friend struct access_type<MetaClassMember>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_class_member_tag categorize_meta_object(MetaClassMember);
};

/// MetaTemplatedClass is a type reflecting instantiation of a class template
/**
 *  @see meta_templated_class_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaTemplatedClass
 : virtual public MetaTemplatedType
 , virtual public MetaClass
{

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_templated_class_tag
	categorize_meta_object(MetaTemplatedClass);
};

/// MetaEnum is a type reflecting enumerations
/**
 *  @see meta_enum_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaEnum
 : virtual public MetaType
 , virtual public MetaScope
{
	/** Meta-function returning a range of MetaEnumValues
	 *  which are reflecting the individual value of the reflected enum
	 *
	 *  @see mp::range
	 *  @returns Range<MetaEnumValue>
	 */
	friend struct enum_values<MetaClass>;

	/// Number of elements in the enumeration
	typedef IntegralConstantType size;

	/// Meta-function returning the count of elements in an enumeration
	/** This meta-function "returns" the same IntegralConstantType as
	 *  the @c size member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns IntegralConstantType
	 */
	friend struct enum_size<MetaEnum>;

	/// Gets the enumerated value with the specified compile-time index
	static UnspecifiedEnumType value(IntegralConstantType index);

	/// Gets the enumerated value with the specified index
	static UnspecifiedEnumType value(int index);

	/// Gets the name of the enumerated value with the specified index
	static std::string value_name(IntegralConstantType index);

	/// Gets the name of the enumerated value with the specified index
	static std::string value_name(int index);

	/// Returns the name of a value
	static std::string name_by_value(Enum x);

	/// Returns an enumerated value by its name
	static Enum value_by_name(const std::string& name);

	/// Checks if there is a value with the provided name
	static bool has_value_name(const std::string& name);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_enum_tag
	categorize_meta_object(MetaEnum);
};

/// MetaEnumValue is a type reflecting individual enumeration values
/**
 *  @see meta_enum_value_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaEnumValue
 : virtual public MetaNamedScopedObject
{
	/// A MetaEnum reflecting the scope of the enumerated value
	typedef MetaEnum scope;

	/// The enumerated value wrapped by an integral_constant type
	typedef IntegralConstantType value;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_enum_value_tag
	categorize_meta_object(MetaEnumValue);
};

/// MetaInheritance is a type reflecting class' base classes inheritance
/**
 *  @see meta_inheritance_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaInheritance
 : virtual public MetaObject
{
	/// Reflects the derived class in this inheritance
	typedef MetaClass derived_class;

	/// Meta-function returning the derived class in an class inheritance
	/** This meta-function "returns" the same MetaClass as
	 *  the @c derived_class member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns MetaClass
	 */
	friend struct derived_class<MetaInheritance>;

	/// The inheritance type tag
	/** Tag reflecting the inheritance type (virtual | non-virtual)
	 *  for the inheritance reflected by this meta_inheritance.
	 *
	 *  @see InheritanceTypeTag
	 */
	typedef InheritanceTypeTag inheritance_type;

	/// Meta-function returning the inheritance type of the base class
	/** This meta-function "returns" the same InheritanceTypeTag as
	 *  the @c inheritance_type member typedef, but is better suited for
	 *  algorithms using lambda placeholder expressions.
	 *  @returns InheritanceTypeTag
	 */
	friend struct inheritance_type<MetaInheritance>;

	/// The base class member access type tag
	/** Tag reflecting the member access type (private|protected|public)
	 *  for the base class inheritance reflected by this meta_inheritance.
	 *
	 *  @see InheritanceTypeTag
	 */
	typedef AccessTypeTag access_type;

	/// Meta-function returning the access type of the base class
	/** This meta-function "returns" the same AccessTypeTag as
	 *  the @c access_type member typedef, but is better suited for
	 *  algorithms using lambda placeholder expressions.
	 *  @returns AccessTypeTag
	 */
	friend struct access_type<MetaInheritance>;

	/// Reflects the base class in this inheritance
	typedef MetaClass base_class;

	/// Meta-function returning the base class in an class inheritance
	/** This meta-function "returns" the same MetaClass as
	 *  the @c base_class member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns MetaClass
	 */
	friend struct base_class<MetaInheritance>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_inheritance_tag categorize_meta_object(MetaInheritance);
};


/// MetaVariable is a type reflecting a variable
/**
 *  @see meta_variable_tag
 *  @see meta_parameter_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaVariable
 : virtual public MetaNamedScopedObject
{
	/// The storage class tag
	/** Tag reflecting the storage class (auto|static|mutable|...)
	 *  for the reflected variable.
	 *
	 *  @see StorageClassTag;
	 */
	typedef StorageClassTag storage_class;

	/// Meta-function returning the storage class of the variable
	/** This meta-function "returns" the same StorageClassTag as
	 *  the @c storage_class member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns StorageClassTag
	 */
	friend struct storage_class<MetaVariable>;

	/// MetaType reflecting the type of the variable
	typedef MetaType type;

	/// Meta-function returning the type of the variable
	/** This meta-function "returns" the same MetaType as
	 *  the @c type member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns MetaType
	 */
	friend struct type_of<MetaVariable>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_variable_tag
	categorize_meta_object(MetaVariable);
};

/// MetaFreeVariable is a type reflecting free variable
/**
 *  @see meta_free_variable_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaFreeVariable
 : virtual public MetaVariable
{
	/// Reflects the namespace to which the reflected variable belongs
	typedef MetaNamespace scope;

	/// Gets the value of the global variable
	/** This function allows to get the value of the global variable
	 *  reflected by this MetaFreeVariable
	 *
	 *  @return The result has the same type as the global variable.
	 */
	static Type get(void);

	/// Sets the value of the free variable
	/** This function allows to set the value of the free variable
	 *  reflected by this MetaFreeVariable to the @a new_value.
	 *
	 *  @tparam Type the type of the value to be set. This type must
	 *  be implicitly convertible to the type of the member variable.
	 *
	 *  @param new_value the new value to be set to the member variable
	 *  of the passed instance.
	 */
	static void set(Type new_value);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_free_variable_tag
	categorize_meta_object(MetaFreeVariable);
};

/// MetaPlainFreeVariable is a type reflecting free variable
/**
 *  @see meta_plain_free_variable_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaPlainFreeVariable
 : virtual public MetaFreeVariable
{
	/// Gets a pointer to the free variable
	/** This function allows to get a pointer to the free variable
	 *  reflected by this MetaPlainFreeVariable
	 *
	 *  @return The result has the same type as the free variable.
	 */
	static Type* address(void);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_plain_free_variable_tag
	categorize_meta_object(MetaPlainFreeVariable);
};

/// MetaMemberVariable is a type reflecting a class' member variable
/**
 *  @see meta_member_variable_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaMemberVariable
 : virtual public MetaClassMember
 , virtual public MetaVariable
{
	/// Gets the value of the member when given an instance of the Class
	/** This function allows to get the value of the member variable
	 *  reflected by this MetaMemberVariable of the provided instance.
	 *
	 *  @tparam Class the type of the parent class to which this member
	 *  variable belongs to.
	 *  @param instance an instance of the @a Class, the value
	 *  of the member variable of which is to be obtained.
	 *  @return The result has the same type as the member variable.
	 */
	static Type get(const Class& instance);

	/// Sets the value of the member when given an instance of the Class
	/** This function allows to set the value of the member variable
	 *  reflected by this MetaMemberVariable of the provided instance
	 *  to the provided @a new_value.
	 *
	 *  @tparam Class the type of the parent class to which this member
	 *  variable belongs to.
	 *  @tparam Type the type of the value to be set. This type must
	 *  be implicitly convertible to the type of the member variable.
	 *
	 *  @param instance an instance of the @a Class, the value
	 *  of the member variable of which is to be modified.
	 *  @param new_value the new value to be set to the member variable
	 *  of the passed instance.
	 */
	static void set(Class& instance, Type new_value);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_member_variable_tag
	categorize_meta_object(MetaMemberVariable);
};

/// MetaPlainMemberVariable is a meta-object reflecting POD class' member variable
/**
 *  @see meta_plain_member_variable_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaPlainMemberVariable
 : virtual public MetaMemberVariable
{
	/// Returns the byte offset of the member inside the class
	static ptrdiff_t offset(void);

	/// Returns a pointer to the member when given a reference to Class
	/** This function allows to get the pointer of the member variable
	 *  reflected by this MetaPlainMemberVariable of the provided instance.
	 *
	 *  @tparam Class the type of the parent class to which this member
	 *  variable belongs to.
	 *
	 *  @param instance an instance of the @a Class
	 */
	static Type* address(Class& instance);

	static const Type* address(const Class& instance);

	/// Returns a class member pointer for the reflected member variable
	static MemberVariablePointer member_pointer(void);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_plain_member_variable_tag
	categorize_meta_object(MetaPlainMemberVariable);
};

/// MetaParameter is a type reflecting a function parameter
/**
 *  @see meta_parameter_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaParameter
 : virtual public MetaVariable
{
	/// The position of the parameter
	typedef IntegralConstantType position;

	/// Meta-function returning the position of a parameter
	/** This meta-function "returns" the same IntegralConstantType as
	 *  the @c position member typedef, but is better suited
	 *  for algorithms using lambda placeholder expressions.
	 *  @returns IntegralConstantType
	 */
	friend struct parameter_position<MetaParameter>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_parameter_tag
	categorize_meta_object(MetaParameter);
};

/// MetaFunction is a meta-object reflecting a function
/**
 *  @see meta_function_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaFunction
 : virtual public MetaScope
{
	/// MetaType reflecting the return type of the function
	typedef MetaType result_type;

	/// Meta-function returning the result type of the function
	/** This meta-function "returns" the same MetaType as
	 *  the @c result_type member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns MetaType
	 */
	friend struct result_type<MetaFunction>;

	/// StorageClassTag indicating the storage class or linkage of function
	/** The linkage typedef is an alternate name for this specifier tag
	 *  @see linkage
	 */
	typedef StorageClassTag storage_class;

	/// StorageClassTag indicating the storage class or linkage of function
	/** The storage_class typedef is an alternate name for this specifier
	 *  @see storage_class
	 */
	typedef StorageClassTag linkage;

	/// Meta-function returning the linkage of the function
	/** This meta-function "returns" the same StorageClassTag as
	 *  the @c linkage member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns StorageClassTag
	 */
	friend struct linkage<MetaFunction>;

	/// Returns a range of MetaVariables reflecting the parameters
	/**
	 *  @see MetaVariable
	 *  @see mp::range
	 *  @returns Range<MetaVariable>
	 */
	friend struct parameters<MetaFunction>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_function_tag
	categorize_meta_object(MetaFunction);
};

/// MetaMemberFunction is a meta-object reflecting a member function
/**
 *  @see meta_member_function_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaMemberFunction
 : virtual public MetaClassMember
 , virtual public MetaFunction
{

	/// Specifies the constness of the member function
	/**
	 *  @see ConstnessTag
	 */
	typedef ConstnessTag constness;

	/// Meta-function returning the constness of the member function
	/** This meta-function "returns" the same ConstnessTag as
	 *  the @c constness member typedef, but is better suited for algorithms
	 *  using lambda placeholder expressions.
	 *  @returns ConstnessTag
	 */
	friend struct constness<MetaMemberFunction>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_member_function_tag
	categorize_meta_object(MetaMemberFunction);
};

/// MetaConstructor reflects a type / class constructor
/**
 *  @see meta_constructor_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaConstructor
 : virtual public MetaFunction
 , virtual public MetaClassMember
{

	/// MetaType reflecting the constructed type
	typedef MetaType result_type;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_constructor_tag
	categorize_meta_object(MetaConstructor);
};

/// MetaConversionOperator reflects a class conversion operator
/**
 *  @see meta_conversion_operator_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaConversionOperator
 : virtual public MetaMemberFunction
{

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_conversion_operator_tag
	categorize_meta_object(MetaConversionOperator);
};

/// MetaOverloadedFunctions reflects a set of functions with the same name in a scope
/**
 *  @see meta_overloaded_functions_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_concepts
 */
struct MetaOverloadedFunctions
 : virtual public MetaNamedScopedObject
{
	/// Returns the range of MetaFunctions reflecting the overloads
	/** This meta-function template returns a range of MetaFunctions
	 *  that reflect the individual overloaded functions in the set.
	 *
	 *  @see MetaFunction
	 *  @see mp::range
	 *  @returns Range<MetaFunction>
	 */
	friend struct overloads<MetaOverloadedFunctions>;
};

MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

