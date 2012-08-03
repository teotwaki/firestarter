/**
 * @file puddle/doc/concepts.hpp
 * @brief Classes modelling the concepts defined by Puddle
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_DOC_CONCEPTS_1104071043_HPP
#define PUDDLE_DOC_CONCEPTS_1104071043_HPP

#include <puddle/config.hpp>

// This section is here for documentation purposes only
#ifdef MIRROR_DOCUMENTATION_ONLY

/** @defgroup puddle_meta_object_concepts Puddle - Meta-object concepts
 *
 *  These classes are models of the concepts defined by Puddle. They are
 *  defined for documentation purposes only and cannot be used in any
 *  end-user code.
 */

PUDDLE_NAMESPACE_BEGIN

struct Integer;
struct Boolean;
struct MetaObject;
struct MetaNamedObject;
struct MetaScopedObject;
struct MetaNamedScopedObject;
struct MetaScope;
struct MetaNamespace;
struct MetaType;
struct MetaClass;
struct MetaEnum;
struct MetaVariable;
struct MetaClassMember;
struct MetaConstructor;
struct MetaContainer;

/// Integral constant type wrapper template
template <typename IntType>
struct IntConst
{
	/// Returns the value of the integer constant type
	IntType value(void) const;

	/// Returns the value of the integer constant type
	/**
	 *  @see value()
	 */
	operator IntType (void) const;

	/// Equality comparison
	friend Boolean operator == (IntConst, IntConst);

	/// Inequality comparison
	friend Boolean operator != (IntConst, IntConst);

	/// Less-equal
	friend Boolean operator <= (IntConst, IntConst);

	/// Greater-equal
	friend Boolean operator >= (IntConst, IntConst);

	/// Less-than
	friend Boolean operator <  (IntConst, IntConst);

	/// Greater-than
	friend Boolean operator >  (IntConst, IntConst);

	/// Addition operator
	IntConst operator ! (void) const;

	/// Addition operator
	friend IntConst operator + (IntConst, IntConst);

	/// Subtraction operator
	friend IntConst operator - (IntConst, IntConst);

	/// Multiplication operator
	friend IntConst operator * (IntConst, IntConst);

	/// Integral division operator
	friend IntConst operator / (IntConst, IntConst);

	/// Bitwise and operator
	friend IntConst operator & (IntConst, IntConst);

	/// Bitwise or operator
	friend IntConst operator | (IntConst, IntConst);

	/// Logical and operator
	friend IntConst operator && (IntConst, IntConst);

	/// Logical or operator
	friend IntConst operator || (IntConst, IntConst);
};

/// Integral constant type wrapper
struct Integer : IntConst<int> { };

/// Boolean constant type wrapper
struct Boolean : IntConst<bool> { };

/// Character constant type wrapper
struct Character : IntConst<char> { };


/// A compile time string of characters
/**
 *  @see ct_string(void)
 *  @see MetaNamedObject
 */
struct CompileTimeString
{
	/// Returns a C-string
	const char* c_str(void) const;

	/// Equality comparison
	friend Boolean operator == (CompileTimeString, CompileTimeString);

	/// Nonequality comparison
	friend Boolean operator != (CompileTimeString, CompileTimeString);

	/// Concatenation operator
	friend CompileTimeString operator + (
		CompileTimeString,
		CompileTimeString
	);

	/// Testing for emptyness
	Boolean empty(void) const;

	/// Returns the size of the string
	Integer size(void) const;

	/// Returns the first character in the string
	Character front(void) const;

	/// Returns the character at the specified @c index
	Character at(Integer index) const;

	/// Returns the character at the specified @c Index
	template <int Index>
	Character at_c(void) const;

	/// Returns true if this string starts with the passed string
	Boolean starts_with(CompileTimeString str) const;

	/// Returns true if this string ends with the passed string
	Boolean ends_with(CompileTimeString str) const;

	/// Returns true if this string contains the passed string
	Boolean contains(CompileTimeString str) const;

	/// Returns this string without the first character
	CompileTimeString step_front(void) const;

	/// Returns this string without the first @c n characters
	CompileTimeString skip_front(Integer n) const;

	/// Returns this string without the first @c N characters
	template <int N>
	CompileTimeString skip_front_c(void) const;

	/// Returns the first @c n characters from this string
	CompileTimeString head(Integer n) const;

	/// Returns the first @c N characters from this string
	template <int N>
	CompileTimeString head_c(void) const;

	/// Returns the last @c n characters from this string
	CompileTimeString tail(Integer n) const;

	/// Returns the last @c N characters from this string
	template <int N>
	CompileTimeString tail_c(void) const;

	/// Returns a substring starting with @c str or empty string
	CompileTimeString find(CompileTimeString str) const;
};

/// A range of Elements
/** This class is a wrapper around the Mirror's compile time range.
 *  @see mirror::mp::range
 */
template <typename Element>
struct Range
{
	/// Returns true if the range is empty
	Boolean empty(void) const;

	/// Returns the number of elements in the range
	Integer size(void) const;

	/// Returns a range without the first element of this range
	/** This function should not be called on empty ranges
	 */
	Range step_front(void) const;

	/// Returns the element at the front of the range
	Element front(void) const;

	/// Returns the element at the specified @a Index
	Element at(Index) const;

	/// Returns the element at the specified @a Index
	template <int Index>
	Element at_c(void) const;

	/// Returns a new range containing only elements satisfying a predicate
	template <class UnaryPredicate>
	Range only_if(UnaryPredicate pred) const;

	/// Executes a functor on every element in the range
	/** This function executes the specified functor on every element
	 *  in this range. The functor must implement a function call operator
	 *  with three parameters. The first parameter is of the Element type
	 *  the second and third are booleans indicating the first and the last
	 *  element in the traversal.
	 *
	 *  @see accumulate
	 */
	template <class Functor>
	void for_each(Functor func) const;

	/// Accumulates the results of a functor called on every element
	/** This function executes the specified functor on every element
	 *  in the range and the result of previous invocation or the initial
	 *  value.
	 *  The functor must implement a function call operator
	 *  with four parameters. The first parameter is of the ValueType type
	 *  - the previous or initial value, the second has the Element type,
	 *  the third and fourth are booleans indicating the first and the last
	 *  element in the traversal.
	 *
	 *  @see accumulate
	 */
	template <typename ResultType, class Functor, typename ValueType>
	ResultType accumulate(Functor func, ValueType init=ResultType()) const;
};

/// Type reflecting tags
struct Tag
{
	/// Equality comparison
	friend Boolean operator == (Specifier, Specifier);

	/// Inequality comparison
	friend Boolean operator != (Specifier, Specifier);

	/// Less-equal
	friend Boolean operator <= (Specifier, Specifier);

	/// Greater-equal
	friend Boolean operator >= (Specifier, Specifier);

	/// Less-than
	friend Boolean operator <  (Specifier, Specifier);

	/// Greater-than
	friend Boolean operator >  (Specifier, Specifier);

	/// Returns false for "non-specifiers" (non-virtual, ...)
	operator bool (void) const;

	/// Returns true for "non-specifiers"
	bool operator ! (void) const;
};

/// Type reflecting specifiers
struct Specifier : Tag
{
	/// Returns the keyword of the specifier
	std::string keyword(void) const;
};

/// Specifies the container kind
/**
 *  @see mirror::ContainerKindTag
 */
struct ContainerKind : Tag
{
};


/// MetaInstance is a type reflecting an instance of a type
/**
 *  @see meta_instance_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaInstance
{
	/// Returns the variable of the instance
	/** This function returns a MetaVariable reflecting the variable
	 *  of the reflected instance
	 */
	MetaVariable variable(void) const;

	/// Returns a range of meta instances reflecting the member instances
	/** If the type of the instance is elaborated, and it has member
	 *  variables, then this function returns a non-empty range
	 *  of MetaInstances, which reflect the individual instances of the
	 *  member variables.
	 */
	Range<MetaInstance> attributes(void) const;

	/// Gets the value of the instance
	Type get(void) const;

	/// Sets a new value to the instance
	void set(Type value) const;

	/// Returns the category of the MetaObject
	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	meta_instance_tag category(void) const;

	/// Returns true if the meta-object is of the specified @a Category
	/** This function returns a true Boolean if the queried category
	 *  is meta_instance tag and returns false Boolean otherwise.
	 *
	 *  @tparam Category a meta-object category tag type
	 *
	 *  @see meta_instance_tag
	 *  @see mirror::meta_object_tag
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	template <typename Category>
	Boolean is_a(Category) const;
};

/// MetaPlainInstance is a type reflecting an instance address of which is known
/**
 *  @see meta_plain_instance_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaPlainInstance
{
	/// Returns the address of the instance
	Type* address(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	meta_plain_instance_tag category(void) const;
};

/// MetaObject is a type defined by the Puddle library to provide meta-data
/**
 *  @see mirror::MetaObject
 *  @see mirror::meta_object_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaObject
{
	/// Equality comparison
	friend Boolean operator == (MetaObject, MetaObject);

	/// Inequality comparison
	friend Boolean operator != (MetaObject, MetaObject);

	/// Returns the category of the MetaObject
	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_object_tag category(void) const;

	/// Returns true if the meta-object is of the specified @a Category
	/**
	 *  @tparam Category a meta-object category tag type
	 *
	 *  @see mirror::meta_object_tag
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	template <typename Category>
	Boolean is_a(Category) const;

};

/// MetaNamedObject is a type reflecting a construct (type, ...) having a name
/** MetaNamedObject reflects base-level constructs which are identified
 *  by a name (identifier). Examples of named objects are types, namespaces,
 *  classes, functions, class members, etc. even if some of them might be
 *  anonymous.
 *  Meta objects reflecting these named objects provide functions for getting
 *  their names.
 *
 *  @see mirror::MetaNamedObject
 *  @see mirror::meta_named_object_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaNamedObject
 : virtual public MetaObject
{
	/// Returns the base name of the reflected entity
	/** This function returns the base name of the reflected
	 *  construct (type, class, namespace, member variable, etc.)
	 *  without the nested name specifier.
	 */
	std::string base_name(void) const;

	/// Returns a compile-time string containing the base_name
	CompileTimeString static_name(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_named_object_tag category(void) const;
};

/// MetaMetaObject is a type reflecting a meta_object
/**
 *  @see mirror::meta_meta_object_tag
 *  @see MetaObject
 *  @see mirror::MetaMetaObject
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaMetaObject
 : virtual public MetaNamedObject
{
	/// Returns the name of the reflected language construct
	/** This function returns a human readable name of the
	 *  language construct (type, class, namespace, member variable,
	 *  etc.) reflected by this meta-object.
	 */
	std::string construct_name(void) const;

	/// Returns the range of generalizations for the reflected MetaObject
	/** This member function returns a range of MetaMetaObjects
	 *  that reflect the generalizations of the MetaObject reflected
	 *  by this MetaMetaObject.
	 */
	Range<MetaMetaObject> generalizations(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_meta_object_tag category(void) const;
};


/// MetaScopedObject is a type reflecting a construct defined in a scope
/**
 *
 *  @see mirror::MetaScopedObject
 *  @see mirror::meta_scoped_object_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaScopedObject
 : virtual public MetaObject
{
	/// Returns a meta-object reflecting the scope of the scoped object
	/** The resulting type conforms to the MetaScope concept
	 *  and provides meta-data about the scope which
	 *  encloses the construct reflected by this MetaScopedObject.
	 *  @see MetaScope
	 */
	MetaScope scope(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_scoped_object_tag category(void) const;
};

/// MetaNamedScopedObject is a type reflecting a named construct defined in a scope
/**
 *
 *  @see mirror::MetaNamedScopedObject
 *  @see mirror::meta_named_scoped_object_tag
 *  @ingroup puddle_meta_object_concepts
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
	std::string full_name(void) const;

	/// Returns the local name of the reflected entity
	/** This function returns the local name of the reflected
	 *  construct (type, class, namespace, member variable, etc.)
	 *  including the partial or full nested name specifier, based
	 *  on whether the parent scope or one of its enclosing scopes
	 *  of the construct reflected by this MetaNamedScopedObject
	 *  is used with the PUDDLE_USING_NAMESPACE macro.
	 *
	 *  @see PUDDLE_USING_NAMESPACE
	 *
	 *  @include typenames_03.cpp
	 */
	std::string local_name(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_named_scoped_object_tag category(void) const;
};

/// MetaScope is a type reflecting a construct enclosing other constructs
/**
 *
 *  @see mirror::MetaScope
 *  @see mirror::meta_scope_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaScope
 : virtual public MetaNamedScopedObject
{
	/// Returns the members of the reflected scope
	Range<MetaNamedScopedObject> members(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_scope_tag category(void) const;
};

/// MetaNamespace is a type reflecting namespaces
/**
 *
 *  @see mirror::MetaNamespace
 *  @see mirror::meta_namespace_tag
 *  @see mirror::meta_global_scope_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaNamespace
 : virtual public MetaScope
{
	/// Returns the nested namespaces in the namespace
	/** Returns a range of MetaNamespace(s)
	 *  which are reflecting the nested namespaces of the reflected namespace.
	 *
	 *  @see Range
	 */
	Range<MetaNamespace> namespaces(void) const;

	/// Returns the free variables in the namespace
	/** Returns a range of MetaFreeVariable(s)
	 *  which are reflecting the free variables of the reflected namespace.
	 *
	 *  @see Range
	 */
	Range<MetaFreeVariable> free_variables(void) const;

	/// Returns a type allowing to access variable meta-data by identifier
	/** Returns a type which provides member functors having the same names
	 *  as the free variables in the reflected namespace. Invoking these
	 *  nullary functors returns MetaFreeVariable reflecting the individual
	 *  free variables.
	 */
	unspecified free_variables_by_name(void) const;

	/// Returns a type allowing to access variable meta-data by identifier
	/** @see free_variables_by_name()
	 */
	unspecified* operator -> (void) const;

	/// Returns the (overloaded) free functions in the namespace
	/** Returns a range of MetaOverloadedFunctions
	 *  which are reflecting the free functions of the reflected namespace.
	 *
	 *  @see Range
	 */
	Range<MetaOverloadedFunctions> free_functions(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_namespace_tag category(void) const;
};

/// MetaType is a type reflecting base-level types
/**
 *
 *  @see mirror::MetaType
 *  @see mirror::meta_type_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaType
 : virtual public MetaNamedScopedObject
{
	/// Function returning the base-level type reflected by this MetaType.
	/** Use decltype to get the type
	 */
	static unspecified original_type(void);

	/// Returns meta-data describing the type's constructors
	/** This function returns a range of MetaConstructor (s)
	 *  which are reflecting the constructors of the reflected type
	 *
	 *  @see Range
	 */
	Range<MetaConstructor> constructors(void) const;

	/// Returns the container kind tag
	/** Tag specifying the container kind of the reflected type.
	 */
	ContainerKind container_kind(void) const;

	/// Returns the meta-data describing element containers
	/** Meta-function returning a range of MetaContainers
	 *  which are reflecting the element containers of the reflected type
	 */
	Range<MetaContainer> containers(void) const;

	/// Apply a unary trait to the original type and reflect the result
	template <template <class> class UnaryTrait>
	unspecified apply(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_type_tag category(void) const;
};

/// MetaTypedef is a type reflecting base-level typedefined types
/**
 *  @see mirror::MetaTypedef
 *  @see mirror::meta_typedef_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaTypedef
 : virtual public MetaType
{
	/// Returns MetaType reflecting the type used in the type definition
	MetaType type(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_typedef_tag category(void) const;
};

/// MetaTemplatedType is a type reflecting instantiation of a type template
/**
 *
 *  @see mirror::MetaTemplatedType
 *  @see mirror::meta_templated_type_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaTemplatedType
 : virtual public MetaType
{
	typedef MetaTypeTemplate type_template;

	/// Returns the template of the templated type
	/** this function returns a MetaTypeTemplate reflecting the template
	 *  which was used for the instantiation of this templated type.
	 *
	 *  @see parameters
	 *  @see MetaTypeTemplate
	 */
	MetaTemplatedType type_template(void) const;

	/// Returns the template parameters
	/** Function returning a range of MetaType(s)
	 *  which are reflecting the individual parameters
	 *  used in the instantiation of the type template.
	 *  @see members
	 *  @see mp::range
	 */
	Range<MetaType> template_parameters(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_templated_type_tag category(void) const;
};

/// MetaTypeTemplate is a type reflecting base-level type templates
/**
 *
 *  @see mirror::MetaTypeTemplate
 *  @see meta_type_template_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaTypeTemplate
 : virtual public MetaNamedScopedObject
{

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_type_template_tag category(void) const;
};

/// MetaClass is a type reflecting base-level classes
/**
 *  @see mirror::MetaClass
 *  @see mirror::meta_class_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaClass
 : virtual public MetaType
 , virtual public MetaScope
{
	/// Returns the elaborated type specifier
	/** Specifier reflecting the elaborate type specifier (class | struct |
	 *  union) that was specified during definition (and registering)
	 *  of the class reflected by this meta_class.
	 */
	Specifier elaborated_type(void) const;

	/// Returns the meta-data describing inheritance of base classes
	/** Function returning a range of MetaInheritance(s)
	 *  which are reflecting the inheritance of the base classes
	 */
	Range<MetaInheritance> base_classes(void) const;

	/// Returns the meta-data describing member variables
	/** Function returning a range of MetaMemberVariable(s)
	 *  which are reflecting the member variables of the reflected class
	 *  not including those inherited from base classes.
	 */
	Range<MetaMemberVariable> member_variables(void) const;

	/// Returns the meta-data describing all member variables
	/** Meta-function returning a range of MetaMemberVariable(s)
	 *  which are reflecting the member variables of the reflected class
	 *  including those inherited from base classes.
	 */
	Range<MetaMemberVariable> all_member_variables(void) const;

	/// Returns a type allowing to access variable meta-data by identifier
	/** Returns a type which provides member functors having the same names
	 *  as the member variables in the reflected class. Invoking these
	 *  nullary functors returns MetaMemberVariable reflecting the individual
	 *  member variables.
	 */
	unspecified member_variables_by_name(void) const;

	/// Returns a type allowing to access variable meta-data by identifier
	/** @see member_variables_by_name()
	 */
	unspecified* operator -> (void) const;

	/// Returns the meta-data describing all element containers
	/** Meta-function returning a range of MetaContainers
	 *  which are reflecting the element containers of the reflected class,
	 *  including the inheited ones.
	 */
	Range<MetaContainer> all_containers(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_class_tag category(void) const;
};

/// MetaClassMember is a type reflecting class member variables and functions
/**
 *  @see mirror::MetaClassMember
 *  @see mirror::meta_class_member_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaClassMember
 : virtual public MetaNamedScopedObject
{
	/// Returns meta-class for the class in which the member is defined
	MetaClass scope(void) const;

	/// Returns the access type specifier
	/** Returns specifier for the access type (private | protected | public)
	 *  for the reflected class member variable, function or constructor.
	 */
	Specifier access_type(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_class_member_tag category(void) const;
};

/// MetaTemplatedClass is a type reflecting instantiation of a class template
/**
 *  @see mirror::MetaTemplatedClass
 *  @see mirror::meta_templated_class_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaTemplatedClass
 : virtual public MetaTemplatedType
 , virtual public MetaClass
{

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_templated_class_tag category(void) const;
};

/// MetaEnum is a type reflecting enumerations
/**
 *  @see mirror::MetaEnum
 *  @see mirror::meta_enum_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaEnum
 : virtual public MetaType
 , virtual public MetaScope
{
	/// Returns the number of values in the reflected enumeration
	Integer size(void) const;

	/// Returns a range of meta-objects reflecting the enumerated values
	/** Meta-function returning a range of MetaEnumValue(s)
	 *  which are reflecting the individual value of the reflected enum
	 */
	Range<MetaEnumValue> enum_values(void) const;

	/// Gets the enumerated value with the specified compile-time index
	UnspecifiedEnumType value(Integer index) const;

	/// Gets the enumerated value with the specified index
	UnspecifiedEnumType value(int index) const;

	/// Gets the name of the enumerated value with the specified index
	std::string value_name(Integer index) const;

	/// Gets the name of the enumerated value with the specified index
	std::string value_name(int index) const;

	/// Returns the name of a value
	std::string name_by_value(Enum x) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_enum_tag category(void) const;
};

/// MetaEnumValue is a type reflecting individual enumeration values
/**
 *  @see mirror::MetaEnumValue
 *  @see mirror::meta_enum_value_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaEnumValue
 : virtual public MetaNamedScopedObject
{
	/// Returns a MetaEnum reflecting the scope of the enumerated value
	MetaEnum scope(void) const;

	/// Returns the enumerated value
	Integer value(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_enum_value_tag category(void) const;
};

/// MetaInheritance is a type reflecting class' base classes inheritance
/**
 *  @see mirror::MetaInheritance
 *  @see mirror::meta_inheritance_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaInheritance
 : virtual public MetaObject
{
	/// Returns the derived class in this inheritance
	MetaClass derived_class(void) const;

	/// Returns the inheritance type specifier
	/** Specifier reflecting the inheritance type (virtual | non-virtual)
	 *  for the inheritance reflected by this meta_inheritance.
	 */
	Specifier inheritance_type(void) const;

	/// Returns the base class member access type tag
	/** Specifier reflecting the member access type (private|protected|public)
	 *  for the base class inheritance reflected by this meta_inheritance.
	 */
	Specifier access_type(void) const;

	/// Returns the base class in this inheritance
	MetaClass base_class(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_inheritance_tag category(void) const;
};


/// MetaVariable is a type reflecting a variable
/**
 *  @see mirror::MetaVariable
 *  @see mirror::meta_variable_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaVariable
 : virtual public MetaNamedScopedObject
{
	/// Returns the storage class tag
	/** Specifier reflecting the storage class (auto|static|mutable|...)
	 *  for the reflected variable.
	 */
	Specifier storage_class(void) const;

	/// Returns MetaType reflecting the type of the variable
	MetaType type(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_variable_tag category(void) const;
};

/// MetaFreeVariable is a type reflecting free variable
/**
 *  @see mirror::MetaFreeVariable
 *  @see mirror::meta_free_variable_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaFreeVariable
 : virtual public MetaVariable
{
	/// Returns the namespace to which the reflected variable belongs
	MetaNamespace scope(void) const;

	/// Gets the value of the global variable
	/** This function allows to get the value of the global variable
	 *  reflected by this MetaFreeVariable
	 *
	 *  @return The result has the same type as the global variable.
	 */
	Type get(void) const;

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
	void set(Type new_value) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_free_variable_tag category(void) const;
};

/// MetaPlainFreeVariable is a type reflecting free variable
/**
 *  @see mirror::MetaPlainFreeVariable
 *  @see mirror::meta_plain_free_variable_tag
 *  @ingroup puddle_meta_object_concepts
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
	Type* address(void);

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_plain_free_variable_tag category(void) const;
};

/// MetaMemberVariable is a type reflecting a class' member variable
/**
 *  @see mirror::MetaMemberVariable
 *  @see mirror::meta_member_variable_tag
 *  @ingroup puddle_meta_object_concepts
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
	Type get(const Class& instance) const;

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
	void set(Class& instance, Type new_value) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_member_variable_tag category(void) const;
};

/// MetaPlainMemberVariable is a meta-object reflecting POD class' member variable
/**
 *  @see mirror::MetaPlainMemberVariable
 *  @see mirror::meta_plain_member_variable_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaPlainMemberVariable
 : virtual public MetaMemberVariable
{
	/// Returns the byte offset of the member inside the class
	ptrdiff_t offset(void) const;

	/// Returns a pointer to the member when given a reference to Class
	/** This function allows to get the pointer of the member variable
	 *  reflected by this MetaPlainMemberVariable of the provided instance.
	 *
	 *  @tparam Class the type of the parent class to which this member
	 *  variable belongs to.
	 *
	 *  @param instance an instance of the @a Class
	 */
	Type* address(Class& instance) const;

	const Type* address(const Class& instance) const;

	/// Returns a class member pointer for the reflected member variable
	MemberVariablePointer member_pointer(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_plain_member_variable_tag category(void) const;
};

/// MetaParameter is a type reflecting a function parameter
/**
 *  @see mirror::MetaParameter
 *  @see mirror::meta_parameter_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaParameter
 : virtual public MetaVariable
{
	/// Returns the position of the parameter
	Integer position(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_parameter_tag category(void) const;
};

/// MetaFunction is a meta-object reflecting a function
/**
 *  @see mirror::MetaFunction
 *  @see mirror::meta_function_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaFunction
 : virtual public MetaScope
{
	/// Returns MetaType reflecting the return type of the function
	MetaType result_type(void) const;

	/// Returns specifier indicating the storage class or linkage of function
	/** The linkage function is an alternate name for this one
	 *  @see linkage
	 */
	Specifier storage_class(void) const;

	/// Returns specifier indicating the storage class or linkage of function
	Specifier linkage(void) const;

	/// Returns a range of MetaVariable(s) reflecting the parameters
	Range<MetaParameter> parameters(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_function_tag category(void) const;
};

/// MetaMemberFunction is a meta-object reflecting a member function
/**
 *  @see mirror::MetaMemberFunction
 *  @see mirror::meta_member_function_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaMemberFunction
 : virtual public MetaClassMember
 , virtual public MetaFunction
{
	/// Function returning the constness specifier of the member function
	Specifier constness(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_member_function_tag category(void) const;
};

/// MetaConstructor reflects a type / class constructor
/**
 *  @see mirror::MetaConstructor
 *  @see mirror::meta_constructor_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaConstructor
 : virtual public MetaFunction
 , virtual public MetaClassMember
{
	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_constructor_tag category(void) const;
};

/// MetaConversionOperator reflects a class conversion operator
/**
 *  @see mirror::MetaConversionOperator
 *  @see mirror::meta_conversion_operator_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaConversionOperator
 : virtual public MetaMemberFunction
{
	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_conversion_operator_tag category(void) const;
};

/// MetaOverloadedFunctions reflects a set of overloaded function in a scope
/**
 *  @see mirror::MetaOverloadedFunctions
 *  @see mirror::meta_overloaded_functions_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaOverloadedFunctions
 : virtual public MetaNamedScopedObject
{
	/// Returns a range of MetaFunctions(s) reflecting the individual overloads
	Range<MetaFunction> overloads(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_overloaded_functions_tag category(void) const;
};

/// MetaTraversal reflects a container element traversal method
/**
 *  @see mirror::MetaTraversal
 *  @see mirror::meta_traversal_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaTraversal
 : virtual public MetaNamedObject
{
	/// The container that this traversal belongs to
	MetaContainer container(void) const;

	/// Starts a new traversal of elements
	mirror::Traversal start(Container container) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_traversal_tag category(void) const;
};

/// MetaLocator reflects a container element location method
/**
 *  @see mirror::MetaLocator
 *  @see mirror::meta_locator_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaLocator
 : virtual public MetaNamedObject
{
	/// The container that this location method belongs to
	MetaContainer container(void) const;

	// TODO: signature
	/// Goes to a location in the specified container
	mirror::Locator go_to(Container container, ...) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_traversal_tag category(void) const;
};

/// MetaInserter reflects a container element insertion method
/**
 *  @see mirror::MetaInserter
 *  @see mirror::meta_inserter_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaInserter
 : virtual public MetaNamedObject
{
	/// The container that this inserter operates on
	MetaContainer container(void) const;

	// TODO: signature, insert

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_inserter_tag category(void) const;
};

/// MetaEraser reflects a container element removal method
/**
 *  @see mirror::MetaEraser
 *  @see mirror::meta_eraser_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaEraser
 : virtual public MetaNamedObject
{
	/// The container that this eraser operates on
	MetaContainer container(void) const;

	// TODO: signature, erase

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_eraser_tag category(void) const;
};

/// MetaContainer reflects a container of elements
/**
 *  @see mirror::MetaContainer
 *  @see mirror::meta_container_tag
 *  @ingroup puddle_meta_object_concepts
 */
struct MetaContainer
 : virtual public MetaScopedObject
{
	/// MetaScope reflecting the class or namespace this container belongs to
	/** The resulting MetaScope is a MetaClass for most containers,
	 *  but it is for example the MetaGlobalScope for array containers.
	 */
	MetaScope scope(void) const;

	/// Returns a MetaType  reflecting the element type of the container
	MetaType element_type(void) const;

	/// Returns a MetaTraversal reflecting the default traversal method
	MetaTraversal default_traversal(void) const;

	/// Returns a Range of MetaTraversals reflecting all traversal methods
	Range<MetaTraversal> traversals(void) const;

	/// Returns a Range of MetaInserters reflecting insertion methods
	Range<MetaInserter> inserters(void) const;

	/// Returns a Range of MetaErasers reflecting element removal methods
	Range<MetaEraser> erasers(void) const;

	/**
	 *  @see mirror::categorize_meta_object
	 *  @see mirror::meta_object_category
	 */
	mirror::meta_container_tag category(void) const;
};

PUDDLE_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

