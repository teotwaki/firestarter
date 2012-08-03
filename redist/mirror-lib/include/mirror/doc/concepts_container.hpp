/**
 * @file mirror/doc/concepts.hpp
 * @brief Classes modelling the container-related concepts defined by Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_1011291729_HPP
#define MIRROR_DOC_CONCEPTS_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/doc/concepts.hpp>

// This section is here for documentation purposes only
#ifdef MIRROR_DOCUMENTATION_ONLY

/** @defgroup meta_container_concepts Mirror - Meta-container concepts
 *
 *  Mirror defines the following concepts for meta-objects providing meta-data
 *  about containers, container element traversals, inserters and erasers.
 */

MIRROR_NAMESPACE_BEGIN

/// Locator allows to get the value of a container's element
/** Locator is not a meta-object, it is an abstraction for types which allow
 *  to access elements in a range or container, like pointers, iterators, etc.
 *  but are restricted to point to a particular element and cannot be made
 *  to point to another element.
 *  Locators also indicate positions inside containers for modifying operations
 *  like element insertion and removal. A particular Locator can only be used
 *  with the same instance of a container type which was used to instantiate
 *  the Locator in the first place.
 *
 *  Some Locators may also refer to special parts of the container (like end())
 *  and do not point to an element. For such locators it is necessary to check
 *  the value of @c dereferencable() before trying to get or possibly set
 *  the value pointed to.
 *  Other locators, for example those returned by Traversal's front() member
 *  function, always point to an element and it is not necessary to check
 *  their dereferencability.
 *
 *  @see SafeLocator
 *  @see MutatingLocator
 *  @see MetaLocator
 */
struct Locator
{
	/// Indicating whether this is a safe locator type
	typedef BooleanConstantType safe;

	/// Indicating whether this is a mutating locator type
	typedef BooleanConstantType mutating;

	/// The type of the element pointed to by this Locator
	typedef unspecified value_type;

	/// Returns true if the locator is dereferencable
	bool dereferencable(void) const;

	/// Shorthand for dereferencable()
	operator bool (void) const;

	/// Shorthand for !dereferencable()
	bool operator !(void) const;

	/// Allows to get the value of the element pointed to by the Locator
	value_type get(void) const;
};

/// A Locator which allows to change the value pointed to
/** Mutating locators allow to modify the value which they refer to.
 *
 *  @see Locator
 *  @see Traversal
 */
struct MutatingLocator
 : public Locator
{
	/// Sets the value of the element pointed to by the Locator
	void set(value_type value) const;
};

/// SafeLocator is a Locator that is always dereferencable
/** When handling SafeLocators it is not necessary to check if they
 *  are dereferencable before calling get and set.
 *
 *  @see Locator
 *  @see Traversal
 */
struct SafeLocator
 : public Locator
{
};

/// Traversal allows to traverse elements of containers in an uniform way
/** Traversal is not a meta-object, it is rather an abstraction for types
 *  wrapping various methods of container element traversal. Instances of these
 *  types are created by the MetaTraversal meta-object and allow the users
 *  to iterate through the elements of an arbitrary container in a generic
 *  way.
 *
 *  @see MetaTraversal
 *  @ingroup meta_container_concepts
 */
struct Traversal
{
	/// Indicates that the element traversal is finished
	/** When this member function returns true, then the step_front
	 *  and the front member functions must not be called.
	 *
	 *  @see empty
	 */
	bool done(void) const;

	/// Synonym for done
	bool empty(void) const;

	/// Returns a locator for the current element in the traversal
	/** This function should not be called if done() returns true.
	 *
	 *  @see done
	 */
	SafeLocator front(void) const;

	/// Moves the front of the range one step ahead, to the next element
	/** This function should not be called if done() returns true.
	 *
	 *  @see done
	 */
	void step_front(void);
};

/// Tag type specifying the kind of container
/**
 *  @see non_container_tag
 *  @see container_tag
 *  @see simple_container_tag
 *  @see single_value_container_tag
 *  @see optional_container_tag
 *  @see variant_container_tag
 *  @see complex_container_tag
 *
 *  @ingroup meta_container_concepts
 */
struct ContainerKindTag;

/// MetaLocator provides meta-data about a location in a container
/**
 *  @see meta_locator_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_container_concepts
 */
struct MetaLocator
 : virtual public MetaNamedObject
{
	/// Signature specifying the kinds of additional parameters for go_to
	/**
	 *  @see container_op_count_param
	 *  @see container_op_value_param
	 *  @see go_to
	 */
	typedef Range<unspecified> signature;

	/// The type of the Locator returned by go_to
	/**
	 *  @see signature
	 */
	typedef Locator locator_type;

	/// Gets a locator for the location reflected by this MetaLocator
	/** This function has one mandatory argument - the container to
	 *  operate on. The other arguments are determined by the signature
	 *  of the locator operation.
	 *
	 *  @see signature
	 */
	static locator_type go_to(Container container, ...);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_locator_tag
	categorize_meta_object(MetaLocator);
};

/// MetaTraversal provides meta-data about a container traversal
/**
 *  @see meta_traversal_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_container_concepts
 */
struct MetaTraversal
 : virtual public MetaNamedObject
{
	/// Signature specifying the kinds of additional parameters for start
	/**
	 *  @see container_op_location_param
	 *  @see container_op_end_location_param
	 *  @see container_op_count_param
	 *  @see container_op_value_param
	 *  @see start
	 */
	typedef Range<unspecified> signature;

	/// The type of the Traversal returned by start
	typedef Traversal traversal_type;

	/// Starts a traversal of the container passed as argument
	/** This function has one mandatory argument - the container to
	 *  operate on. The other arguments are determined by the signature
	 *  of the traversal operation.
	 *
	 *  @see signature
	 */
	static traversal_type start(Container container, ...);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_traversal_tag
	categorize_meta_object(MetaTraversal);
};

/// MetaInserter provides meta-data about a container element insertion method
/**
 *  @see meta_inserter_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_container_concepts
 */
struct MetaInserter
 : virtual public MetaNamedObject
{
	/// Signature specifying the kinds of additional parameters for insert
	/**
	 *  @see container_op_location_param
	 *  @see container_op_count_param
	 *  @see container_op_value_param
	 *  @see insert
	 */
	typedef Range<unspecified> signature;

	/// Inserts one or several values into a container
	/** This function has one mandatory argument - the container to
	 *  operate on. The other arguments are determined by the signature
	 *  of the element insertion operation.
	 *
	 *  @see signature
	 */
	static void insert(Container& container, ...);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_inserter_tag
	categorize_meta_object(MetaInserter);
};

/// MetaEraser provides meta-data about a container element removal method
/**
 *  @see meta_eraser_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_container_concepts
 */
struct MetaEraser
 : virtual public MetaNamedObject
{
	/// Signature specifying the kinds of additional parameters for insert
	/**
	 *  @see container_op_location_param
	 *  @see container_op_end_location_param
	 *  @see container_op_count_param
	 *  @see container_op_value_param
	 *  @see erase
	 */
	typedef Range<unspecified> signature;


	/// Removes one or several values from a container
	/** This function has one mandatory argument - the container to
	 *  operate on. The other arguments are determined by the signature
	 *  of the element removal operation.
	 *
	 *  @see signature
	 */
	static void erase(Container& container, ...);

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_eraser_tag
	categorize_meta_object(MetaEraser);
};

/// MetaContainer provides meta-data about a container
/**
 *  @see meta_container_tag
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_container_concepts
 */
struct MetaContainer
 : virtual public MetaScopedObject
{
	/// MetaScope reflecting the class or namespace this container belongs to
	/** The resulting MetaScope is a MetaClass for most containers,
	 *  but it is for example the MetaGlobalScope for array containers.
	 */
	typedef MetaScope scope;

	/// Checks if the container is empty
	static bool empty(Container& container);

	/// Returns the number of elements in the container if possible
	/** This member function returns the number of elements in the container
	 *  if this information is available for this Container type.
	 *  If the info is not available this function returns zero.
	 */
	static size_t size_hint(Container& container);

	/// MetaType reflecting the type of the elements stored in the container
	typedef MetaType element_type;

	/// The default element traversal method for the reflected container
	typedef MetaTraversal default_traversal;

	/// The default element traversal method for the reflected container
	/** Meta-function returning a MetaTraversal
	 *  which is providing meta-data about the default element traversal
	 *  method for the container reflected by this MetaContainer
	 *
	 *  @see traversals
	 *  @returns MetaTraversal
	 */
	friend struct default_traversal<MetaContainer>;

	/// The element traversal methods available for the reflected container
	/** Meta-function returning a range of MetaTraversals
	 *  which are providing meta-data about all element traversal
	 *  methods for the container reflected by this MetaContainer
	 *
	 *  @see default_traversal
	 *  @see mp::range
	 *  @returns Range<MetaTraversal>
	 */
	friend struct traversals<MetaContainer>;

	/// The element location methods available for the reflected container
	/** Meta-function returning a range of MetaLocators
	 *  which are providing meta-data about all element location
	 *  methods for the container reflected by this MetaContainer
	 *
	 *  @see mp::range
	 *  @returns Range<MetaLocator>
	 */
	friend struct locators<MetaContainer>;

	/// The element insertion methods available for the reflected container
	/** Meta-function returning a range of MetaInserters
	 *  which are providing meta-data about all element insertion
	 *  methods for the container reflected by this MetaContainer
	 *
	 *  @see default_inserter
	 *  @see mp::range
	 *  @returns Range<MetaInserter>
	 */
	friend struct inserters<MetaContainer>;

	/// The element removal methods available for the reflected container
	/** Meta-function returning a range of MetaErasers
	 *  which are providing meta-data about all element removal
	 *  methods for the container reflected by this MetaContainer
	 *
	 *  @see default_inserter
	 *  @see mp::range
	 *  @returns Range<MetaEraser>
	 */
	friend struct erasers<MetaContainer>;

	/** @see meta_object_category
	 *  @see categorize_meta_object(const T&)
	 */
	friend meta_container_tag
	categorize_meta_object(MetaContainer);
};

MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

