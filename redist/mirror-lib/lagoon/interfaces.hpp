/**
 * @file lagoon/interfaces.hpp
 * @brief Definitions of the run-time interfaces of meta-objects
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_INTERFACES_1011291729_HPP
#define LAGOON_INTERFACES_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/categories.hpp>
#include <lagoon/specifiers.hpp>
#include <lagoon/utils.hpp>
#include <puddle/categories.hpp>
#include <mirror/type_traits.hpp>
#include <mirror/meta_object_tags.hpp>
#include <string>
#include <memory>

LAGOON_NAMESPACE_BEGIN

/** @defgroup lagoon_interfaces Lagoon - Interfaces
 *
 *  Lagoon defines several polymorphic abstract interfaces,
 *  through which the meta-data are accessed. There interfaces
 *  are run-time equivalents of Mirror's concepts.
 */


/// The interface for reflecting Lagoon's meta-objects
/** This interface can be used to get meta-data about
 *  a Lagoon's meta-object.
 *
 *  @see meta_object_category
 *  @see mirror::MetaMetaObject
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_meta_object
{
private:
	const std::string _base_name;
	const std::string _ctrt_name;
	meta_object_category _category;

	static inline bool category_matches(unsigned a, unsigned b)
	{
		return (a & b) == b;
	}
protected:
	template <typename MetaObjectTag>
	static meta_object_category get_category(MetaObjectTag tag)
	{
		return puddle::get_category(tag);
	}

	template <typename MetaMetaObject, typename Interface>
	friend shared<Interface> aux::get_meta_object(mirror::meta_meta_object_tag);

	template <typename MetaMetaObject>
	inline meta_meta_object(MetaMetaObject)
	 : _base_name(MetaMetaObject::base_name())
	 , _ctrt_name(MetaMetaObject::construct_name())
	 , _category(get_category(typename MetaMetaObject::category()))
	{ }

public:
#if LAGOON_POLYMORPHIC_MMO
	virtual ~meta_meta_object(void)
	{ }
#endif

	/// Returns the base name of the reflected meta_object
	/** This member function returns the base name
	 *  i.e. the name without the nested name specifier
	 *  of the construct reflected by this meta-object.
	 */
	std::string base_name(void) const
	{
		return _base_name;
	}

	/// Returns the name of the reflected language construct
	/** This function returns a human readable name of the
	 *  language construct (type, class, namespace, member variable,
	 *  etc.) reflected by this meta-object.
	 */
	std::string construct_name(void) const
	{
		return _ctrt_name;
	}

#if LAGOON_MMO_WITH_GENERALIZATIONS || MIRROR_DOCUMENTATION_ONLY
	/// Returns the range of generalizations for the reflected MetaObject
	/** This member function returns a range of meta_meta_objects
	 *  that reflect the generalizations of the meta-object reflected
	 *  by this meta_meta_object.
	 *
	 *  This member function is available only if the
	 *  #LAGOON_MMO_WITH_GENERALIZATIONS preprocessor symbol
	 *  is set to a non-zero integer.
	 */
	virtual range<meta_meta_object> generalizations(void) const = 0;
#endif

	/** @name Reflected meta-object categorization functions
	 *  These functions allow to determine the category of the meta_object
	 *  reflected by this meta_meta_object.
	 */
	//@{

	/// Returns the category of the reflected meta_object
	meta_object_category category(void) const
	{
		return _category;
	}

	/// Returns true if the reflected meta-object is of the cat category
	inline bool is_a(meta_object_category cat) const
	{
		return category_matches(
			unsigned(category()),
			unsigned(cat)
		);
	}

#define LAGOON_HELPER_MAKE_MMO_IS_META_OBJECT(OBJECT, I, X) \
	inline bool is_meta_## OBJECT(void) const \
	{ \
		return is_a(meta_object_category::meta_ ## OBJECT ## _tag); \
	}
MIRROR_FOR_EACH_META_OBJECT(LAGOON_HELPER_MAKE_MMO_IS_META_OBJECT, _)
#undef LAGOON_HELPER_MAKE_MMO_IS_META_OBJECT
	//@}
};

LAGOON_NAMESPACE_END

#if LAGOON_MT_WITH_MAKE_FACTORY
#include <lagoon/polymorph_factory.hpp>
#endif

LAGOON_NAMESPACE_BEGIN

/// The base interface for all Lagoon's meta-objects
/** This interface serves as a common ancestor for all
 *  other run-time meta-object interfaces.
 *
 *  @see meta_meta_object
 *  @see meta_object_category
 *  @see mirror::MetaObject
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_object
{
protected:
	// Helper function used in the cast_to function
	static void* _sel_ptr(void)
	{
		return nullptr;
	}

	// Helper function used in the cast_to function
	template <typename ... P>
	static void* _sel_ptr(void* p,  P* ... pp)
	{
		if(p != nullptr) return p;
		return _sel_ptr(pp...);
	}

#if MIRROR_NO_RTTI
// Helper macro used to implement the cast_to member function
#define LAGOON_HLPR_MO_IMPL_CAST_TO_CALL_BASE(OBJECT, X) \
	meta_ ## OBJECT::cast_to(cat)

// Helper macro used to implement the cast_to member function
#define LAGOON_HLPR_MO_IMPL_CAST_TO(OBJECT) \
	virtual void* cast_to(meta_object_category cat) \
	{ \
		if(cat == meta_object_category::meta_##OBJECT##_tag) \
			return static_cast<meta_##OBJECT*>(this); \
		return _sel_ptr( \
			MIRROR_FOR_EACH_META_OBJECT_BASE( \
				OBJECT, \
				LAGOON_HLPR_MO_IMPL_CAST_TO_CALL_BASE, \
				_, \
				MIRROR_PP_COMMA \
			) \
		); \
	}
#else
#define LAGOON_HLPR_MO_IMPL_CAST_TO(OBJECT)
#endif

public:

	// virtual destructor assuring proper cleanup
	virtual ~meta_object(void) { }

	/// returns meta-data about this meta-object
	virtual shared<meta_meta_object> self(void) const = 0;

	LAGOON_HLPR_MO_IMPL_CAST_TO(object)

	/** @name Reflected construct categorization functions
	 *  These functions allow to determine the category of the
	 *  base level construct reflected by the meta_object
	 */
	//@{

	/// Always returns true
#define LAGOON_HELPER_MO_IS_OBJECT(OBJECT, I, X) \
	inline bool is_## OBJECT(void) const \
	{ \
		return self()->is_meta_ ## OBJECT(); \
	}
MIRROR_FOR_EACH_META_OBJECT(LAGOON_HELPER_MO_IS_OBJECT, _)
#undef LAGOON_HELPER_MO_IS_OBJECT

	//@}
};

/** This interface provides meta-data about base-level
 *  constructs having a name.
 *
 *  @see mirror::MetaNamedObject
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_named_object
 : virtual meta_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(named_object)

	/// Returns the base name of the reflected construct
	/** This member function returns the base name
	 *  i.e. the name without the nested name specifier
	 *  of the construct reflected by this meta-object.
	 */
	virtual std::string base_name(void) = 0;
};

/** This interface provides meta-data about base-level constructs
 *  which are defined inside of a scope like namespace or class.
 *
 *  @see mirror::MetaScopedObject
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_scoped_object
 : virtual meta_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(scoped_object)

	/// Returns a meta-object reflecting a scope of this object
	virtual shared<meta_scope> scope(void) = 0;
};

/** This interface provides meta-data about named base-level constructs
 *  which are defined inside of a scope like a namespace or a class.
 *
 *  @see mirror::MetaNamedScopedObject
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_named_scoped_object
 : virtual meta_named_object
 , virtual meta_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(named_scoped_object)

	/// Returns the full name of the reflected construct
	virtual std::string full_name(void) = 0;


#if LAGOON_MNSO_WITH_LOCAL_NAME || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns the local name of the reflected construct
	/**
	 *  Available only if the #LAGOON_MNSO_WITH_LOCAL_NAME preprocessor
	 *  symbol is set to a nonzero integer value.
	 *
	 *  @see base_name
	 *  @see full_name
	 */
	virtual std::string local_name(void) = 0;
#endif
};

/** This interface provides meta-data about named constructs like
 *  namespaces, classes, etc. which can contain other constructs.
 *
 *  @see mirror::MetaScope
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_scope
 : virtual meta_named_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(scope)

	/// This member function returns the members of the scope
	virtual range<meta_named_scoped_object> members(void) = 0;

	/// Convenience member function for finding a member by base_name
	/**
	 *  @tparam Interface the required interface of the member
	 *  @param name the base name to search the member by
	 */
	template <typename Interface>
	shared<Interface> find_member(const std::string& name)
	{
		auto r(this->members());
		while(!r.empty())
		{
			if(r.front()->base_name() == name)
				return r.front().as<Interface>();
			r.step_front();
		}
		return shared<Interface>();
	}
};

/** This interface provides meta-data about usually unnamed scopes
 *   which can contain other constructs.
 *
 *  @see mirror::MetaScope
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_unspecified_scope
 : virtual meta_scope
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(unspecified_scope)
};

/** This interface provides meta-data about a namespace
 *
 *  @see mirror::MetaNamespace
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_namespace
 : virtual meta_scope
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(namespace)

	/// Returns the nested namespaces of this namespace
	virtual range<meta_namespace> namespaces(void) = 0;

	/// Returns the free variables of this namespace
	virtual range<meta_free_variable> free_variables(void) = 0;

	/// Returns the (overloaded) free functions of this namespace
	virtual range<meta_overloaded_functions> free_functions(void) = 0;

	/// Convenience function returning a meta_variable by name
	inline shared<meta_free_variable> variable_(const std::string& name)
	{
		return find_member<meta_free_variable>(name);
	}

	/// Convenience function returning a meta_namespace by name
	inline shared<meta_namespace> namespace_(const std::string& name)
	{
		return find_member<meta_namespace>(name);
	}

	/// Convenience function returning a meta_type by name
	inline shared<meta_type> type_(const std::string& name)
	{
		return find_member<meta_type>(name);
	}

	/// Convenience function returning a meta_class by name
	inline shared<meta_class> class_(const std::string& name)
	{
		return type_(name).as<meta_class>();
	}

	/// Convenience function returning a meta_typedef by name
	inline shared<meta_typedef> typedef_(const std::string& name)
	{
		return find_member<meta_typedef>(name);
	}

	/// Convenience function returning a meta_type_template by name
	inline shared<meta_type_template> template_(const std::string& name)
	{
		return find_member<meta_type_template>(name);
	}
};

/** This interface provides meta-data about the global scope
 *
 *  @see mirror::MetaNamespace
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_global_scope
 : virtual meta_namespace
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(global_scope)
};

/** This interface provides meta-data about a type
 *
 *  @see mirror::MetaType
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_type
 : virtual meta_named_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(type)

	/// Returns meta-constructors reflecting type's constructors
	virtual range<meta_constructor> constructors(void) = 0;

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the type is default constructible
	/**
	 *  Available only if either the
	 *  #LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_default_constructible(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the type is copy constructible
	/**
	 *  Available only if either the
	 *  #LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_copy_constructible(void) = 0;
#endif

#if LAGOON_MT_WITH_NEW || \
    MIRROR_DOCUMENTATION_ONLY
	/// Allocates and constructs a new instance of the reflected type
	/** This member function returns a pointer to a newly allocated
	 *  and default-constructed instance of the type reflected by
	 *  this meta-type if the base-level type has a default constructor.
	 *  If the reflected type does not have a default constructor, this
	 *  function returns a null pointer.
	 *
	 *  Available only if the #LAGOON_MT_WITH_NEW preprocessor
	 *  symbol is set to a nonzero integer value.
	 *
	 *  @see mirror::is_default_constructible
	 *  @see new_copy
	 *  @see delete_
	 *  @see make_factory
	 */
	virtual raw_ptr new_(void) = 0;
#endif

#if LAGOON_MT_WITH_NEW_COPY || \
    MIRROR_DOCUMENTATION_ONLY
	/// Allocates and copy-constructs a new instance of the reflected type
	/** This member function returns a pointer to a newly allocated
	 *  and copy-constructed instance of the type reflected by
	 *  this meta-type if the base-level type has a copy constructor and
	 *  a valid pointer to an instance to copy from is provided.
	 *  If the reflected type does not have a default constructor, this
	 *  function returns a null pointer.
	 *
	 *  Available only if the #LAGOON_MT_WITH_NEW_COPY preprocessor
	 *  symbol is set to a nonzero integer value.
	 *
	 *  @param source a valid pointer to an instance to copy from. If
	 *  a null pointer is provided or the type does not match
	 *  the constructed type the result is undefined. Most probably
	 *  this will cause the application to be aborted in debug builds.
	 *
	 *  @see mirror::is_copy_constructible
	 *  @see new_copy
	 *  @see delete_
	 *  @see make_factory
	 */
	virtual raw_ptr new_copy(raw_ptr source) = 0;
#endif

#if LAGOON_MT_WITH_DEFAULT || \
    MIRROR_DOCUMENTATION_ONLY
	/// Creates a default instance of the type reflected by this meta type
	/** This member function returns a default-constructed instance
	 *  of the type reflected by this meta-type if the base-level type
	 *  has a default constructor.
	 *  If the reflected type does not have a default constructor, this
	 *  function returns an empty boost::any.
	 *
	 *  Available only if the #LAGOON_MT_WITH_DEFAULT
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual boost::any default_(void) = 0;
#endif

#if LAGOON_MT_WITH_DELETE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Deletes an object constructed by new_, new_copy or by a factory
	/**
	 *  Available only if the #LAGOON_MT_WITH_DELETE preprocessor
	 *  symbol is set to a nonzero integer value.
	 *
	 *  @see new_
	 *  @see new_copy
	 *  @see make_factory
	 *  @see make_shared
	 *  @see make_unique
	 */
	virtual void delete_(raw_ptr ptr) = 0;
#endif

#if LAGOON_MT_WITH_MAKE_SHARED || \
    MIRROR_DOCUMENTATION_ONLY
	/// Make a shared raw pointer from an unmanaged raw pointer
	/**
	 *  Available only if the #LAGOON_MT_WITH_MAKE_SHARED
	 *  preprocessor symbol is set to a nonzero integer value.
	 *
	 *  @see new_
	 *  @see new_copy
	 *  @see make_factory
	 *  @see make_unique
	 */
	virtual shared_raw_ptr make_shared(raw_ptr source) = 0;
#endif

#if LAGOON_MT_WITH_MAKE_UNIQUE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Make a shared raw pointer from an unmanaged raw pointer
	/**
	 *  Available only if the #LAGOON_MT_WITH_MAKE_UNIQUE
	 *  preprocessor symbol is set to a nonzero integer value.
	 *
	 *  @see new_
	 *  @see new_copy
	 *  @see make_factory
	 *  @see make_shared
	 */
	virtual unique_raw_ptr make_unique(raw_ptr source) = 0;
#endif

#if LAGOON_MT_WITH_MAKE_FACTORY || \
    MIRROR_DOCUMENTATION_ONLY
	/// Make a polymorphic factory, built by the passed factory builder
	/**
	 *  Available only if the #LAGOON_MT_WITH_MAKE_FACTORY
	 *  preprocessor symbol is set to a nonzero integer value.
	 *  @see new_
	 *  @see new_copy
	 *  @see make_shared
	 *  @see make_unique
	 */
	virtual std::unique_ptr<polymorph_factory> make_factory(
		polymorph_factory_builder& builder,
		raw_ptr build_data
	) = 0;
#endif

	/** @name Reflected type-trait functions
	 *  These functions allow to inspect individual type traits
	 *  of the type reflected by this meta_type.
	 */
	//@{

	/// Returns the size of this type
	virtual std::size_t size_of(void) = 0;

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_ALIGNMENT_OF || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns the alignment of this type
	/**
	 *  Available only if either the #LAGOON_MT_WITH_ALIGNMENT_OF
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual std::size_t alignment_of(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_ARRAY || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the reflected type is an array type
	/**
	 *  Available only if either the #LAGOON_MT_WITH_IS_ARRAY
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_array(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_CONST || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the reflected type is const
	/**
	 *  Available only if either the #LAGOON_MT_WITH_IS_CONST
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_const(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_POINTER || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the reflected type is a pointer
	/**
	 *  Available only if either the #LAGOON_MT_WITH_IS_POINTER
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_pointer(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_REFERENCE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the reflected type is a reference
	/**
	 *  Available only if either the #LAGOON_MT_WITH_IS_REFERENCE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_reference(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_IS_VOLATILE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns true if the reflected type is volatile
	/**
	 *  Available only if either the #LAGOON_MT_WITH_IS_VOLATILE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual bool is_volatile(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_ADD_CONST || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type with added const qualifier
	/**
	 *  Available only if either the #LAGOON_MT_WITH_ADD_CONST
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> add_const(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_ADD_CV || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type with added const and volatile qualifiers
	/**
	 *  Available only if either the #LAGOON_MT_WITH_ADD_CV
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> add_cv(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_ADD_VOLATILE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type with added volatile qualifier
	/**
	 *  Available only if either the #LAGOON_MT_WITH_ADD_VOLATILE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> add_volatile(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_REMOVE_CONST || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type without the const qualifier
	/**
	 *  Available only if either the #LAGOON_MT_WITH_REMOVE_CONST
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> remove_const(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_REMOVE_CV || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type without the const and volatile qualifiers
	/**
	 *  Available only if either the #LAGOON_MT_WITH_REMOVE_CV
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> remove_cv(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_REMOVE_POINTER || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type with one level of pointer indirection removed
	/**
	 *  Available only if either the #LAGOON_MT_WITH_REMOVE_POINTER
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> remove_pointer(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_REMOVE_REFERENCE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type with one level of reference indirection removed
	/**
	 *  Available only if either the #LAGOON_MT_WITH_REMOVE_REFERENCE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> remove_reference(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_REMOVE_VOLATILE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type without the volatile qualifier
	/**
	 *  Available only if either the #LAGOON_MT_WITH_REMOVE_VOLATILE
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> remove_volatile(void) = 0;
#endif

#if LAGOON_ALL_TYPE_TRAITS || \
    LAGOON_MT_WITH_REMOVE_EXTENT || \
    MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type with an array extent removed
	/**
	 *  Available only if either the #LAGOON_MT_WITH_REMOVE_EXTENT
	 *  or the #LAGOON_ALL_TYPE_TRAITS
	 *  preprocessor symbol is set to a nonzero integer value.
	 */
	virtual shared<meta_type> remove_extent(void) = 0;
#endif
	//@}
};

/** This interface provides meta-data about a type template
 *
 *  @see mirror::MetaTypeTemplate
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_type_template
 : virtual meta_named_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(type_template)

};

/// Interface for reflecting typedefined type
/** This interface provides meta-data about a typedef
 *
 *  @see mirror::MetaTypedef
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_typedef
 : virtual meta_type
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(typedef)

	/// Returns a meta_type reflecting this typedef's base type
	virtual shared<meta_type> type(void) = 0;
};

/** This interface provides meta-data about a templated type
 *
 *  @see mirror::MetaTemplatedType
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_templated_type
 : virtual meta_type
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(templated_type)

	/// Returns a meta_type_template reflecting this type's template
	virtual shared<meta_type_template> type_template(void) = 0;

	/// Returns a range of meta_type(s) reflecting template parameters
	virtual range<meta_type> template_parameters(void) = 0;
};

/** This interface provides meta-data about the inheritance of a base-class
 *  of another class.
 *
 *  @see mirror::MetaInheritance
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_inheritance
 : virtual meta_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(inheritance)

	/// returns the inheritance type specifier
	virtual shared<inheritance_type_specifier> inheritance_type(void) = 0;

	/// returns the member access type specifier
	virtual shared<access_type_specifier> access_type(void) = 0;

	/// Returns a meta_class reflecting the base class in the inhertiance
	virtual shared<meta_class> base_class(void) = 0;

	/// Returns a meta_class reflecting the derived class in the inhertiance
	virtual shared<meta_class> derived_class(void) = 0;
};

/** This interface provides meta-data about a class member
 *
 *  @see mirror::MetaClassMember
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_class_member
 : virtual meta_named_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(class_member)

	/// Returns the access type specifier of the class member
	virtual shared<access_type_specifier> access_type(void) = 0;
};

/** This interface provides meta-data about a variable
 *
 *  @see mirror::MetaVariable
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_variable
 : virtual meta_named_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(variable)

	/// returns the storage class specifier
	virtual shared<storage_class_specifier> storage_class(void) = 0;

	/// Returns a meta_type reflecting the type of the variable
	virtual shared<meta_type> type(void) = 0;
};

/** This interface provides meta-data about a free variable
 *
 *  @see mirror::MetaFreeVariable
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_free_variable
 : virtual meta_variable
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(free_variable)

	// TODO: get/set
};

/** This interface provides meta-data about a free variable
 *
 *  @see mirror::MetaPlainFreeVariable
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_plain_free_variable
 : virtual meta_free_variable
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(plain_free_variable)
	// TODO: ref/address
};

/** This interface provides meta-data about a class member variable (attribute)
 *
 *  @see mirror::MetaMemberVariable
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_member_variable
 : virtual meta_variable
 , virtual meta_class_member
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(member_variable)
};

/** This interface provides meta-data about a function or constructor parameter
 *
 *  @see mirror::MetaParameter
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_parameter
 : virtual meta_variable
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(parameter)

	/// Returns the position of the parameter
	virtual int position(void) = 0;
};

/** This interface provides meta-data about a class
 *
 *  @see mirror::MetaClass
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_class
 : virtual meta_type
 , virtual meta_scope
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(class)

	/// returns the elaborated type specifier
	virtual shared<elaborated_type_specifier> elaborated_type(void) = 0;

	/// This member function returns the base clases of this class
	virtual range<meta_inheritance> base_classes(void) = 0;

	/// This member function returns the layout of this class
	virtual range<meta_class> class_layout(void) = 0;

	/// This function returns the member variables of this class
	virtual range<meta_member_variable> member_variables(void) = 0;

#if LAGOON_MC_WITH_ALL_MEMBER_VARIABLES || \
    MIRROR_DOCUMENTATION_ONLY
	/// This function returns all member variables of this class
	/**
	 *  Available only if the #LAGOON_MC_WITH_ALL_MEMBER_VARIABLES
	 *  preprocessor symbol is set to a nonzero integer value.
	 *
	 *  @see member_variables
	 */
	virtual range<meta_member_variable> all_member_variables(void) = 0;
#endif

	/// Returns the (overloaded) member functions of this class
	virtual range<meta_overloaded_functions> member_functions(void) = 0;

	/// This function returns the conversion operators of this class
	virtual range<meta_conversion_operator> conversions(void) = 0;

};

/** This interface provides meta-data about a class
 *
 *  @see mirror::MetaTemplatedClass
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_templated_class
 : virtual meta_templated_type
 , virtual meta_class
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(templated_class)
};

/** This interface provides meta-data about an enum class
 *
 *  @see mirror::MetaEnum
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_enum
 : virtual meta_type
 , virtual meta_scope
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(enum)

	/// The number of elements in the enumeration
	virtual int size(void) = 0;

	/// Return the value at the given index
	virtual int value(int index) = 0;

	/// Returns the name of the i-th value
	virtual std::string value_name(int index) = 0;

	/// Return the value with the provided name
	virtual int value_by_name(const std::string& name) = 0;

	/// Return the name with the provided value
	virtual std::string name_by_value(int value) = 0;

	/// Returns true if the enum has a value with the provided name
	virtual bool has_value_name(const std::string& name) = 0;
};

/** This interface provides meta-data about a set of overloaded functions
 *
 *  @see mirror::MetaOverloadedFunctions
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_overloaded_functions
 : virtual meta_named_scoped_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(overloaded_functions)

	/// returns the meta-function reflecting the individual overloads
	virtual range<meta_function> overloads(void) = 0;
};

/** This interface provides meta-data about a function
 *
 *  @see mirror::MetaFunction
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_function
 : virtual meta_scope
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(function)

	/// returns the meta-type reflecting the type of the result value
	virtual shared<meta_type> result_type(void) = 0;

	/// returns the meta-variables reflecting function's parameters
	virtual range<meta_parameter> parameters(void) = 0;
};

/** This interface provides meta-data about a constructor
 *
 *  @see mirror::MetaConstructor
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_constructor
 : virtual meta_function
 , virtual meta_class_member
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(constructor)

	//virtual raw_ptr new_(...) = 0;

	/// returns the constructor access type specifier
	virtual shared<access_type_specifier> access_type(void) = 0;
};

/** This interface provides meta-data about a member function
 *
 *  @see mirror::MetaMemberFunction
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_member_function
 : virtual meta_function
 , virtual meta_class_member
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(member_function)

	/// returns the storage class specifier
	virtual shared<storage_class_specifier> storage_class(void) = 0;

	/// alternate name for the storage_class member function
	inline shared<storage_class_specifier> linkage(void)
	{
		return this->storage_class();
	}

	/// returns the constness specifier
	virtual shared<constness_specifier> constness(void) = 0;
};

/** This interface provides meta-data about a conversion operator
 *
 *  @see mirror::MetaConversionOperator
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_conversion_operator
 : virtual meta_member_function
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(conversion_operator)
};

/**
 *  @see mirror::MetaLocator
 *
 *  @ingroup lagoon_interfaces
 */
struct meta_locator
 : virtual meta_named_object
{
	LAGOON_HLPR_MO_IMPL_CAST_TO(locator)
};

#ifdef LAGOON_HLPR_MO_IMPL_CAST_TO_CALL_BASE
#undef LAGOON_HLPR_MO_IMPL_CAST_TO_CALL_BASE
#endif
#undef LAGOON_HLPR_MO_IMPL_CAST_TO

LAGOON_NAMESPACE_END

#endif //include guard

