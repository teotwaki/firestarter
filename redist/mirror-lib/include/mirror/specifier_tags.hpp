/**
 *  @file mirror/specifier_tags.hpp
 *  @brief Declaration of tags for various specifier keywords like
 *  virtual, public/protected/private, static, etc.
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_SPECIFIER_TAGS_1011291729_HPP
#define MIRROR_SPECIFIER_TAGS_1011291729_HPP

#include <mirror/preprocessor.hpp>
#include <mirror/meta_prog/range.hpp>
#include <string>

MIRROR_NAMESPACE_BEGIN

/** @defgroup specifier_tags Mirror - Specifier tags
 *
 *  Tag types designating various base-level construct specifiers
 *  like virtual, public/protected/private, static, etc.
 */

#define MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS(KEYWORD_STR, UID) \
	static std::string keyword(void) \
	{ \
		return std::string(KEYWORD_STR, sizeof(KEYWORD_STR) - 1); \
	} \
	static std::integral_constant<int, UID> _uid(void)\
	{ \
		return std::integral_constant<int, UID>(); \
	}


/// Tag for a "non-specifier"
/** This tag is used by compile-time meta-objects that can have
 *  a specifier but do not have one. For example a member function
 *  can but does not have to be declared with a virtual, static or
 *  const specifier.
 *
 *  @ingroup specifier_tags
 */
struct spec__tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("", __LINE__)};
typedef spec__tag spec_none_tag;

#define MIRROR_FOR_EACH_SPEC_BASE_none(MACRO, DATA, SEP_MACRO)


/// Common base for inheritance type tags
/**
 *  @ingroup specifier_tags
 */
struct spec_inheritance_type_tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_inheritance_type(MACRO, DATA, SEP_MACRO)
#define MIRROR_FOR_EACH_SPEC_TYPE_inheritance_type(MACRO, DATA, SEP_MACRO) \
	MACRO(virtual, DATA) \
	SEP_MACRO() \
	MACRO(non_virtual, DATA)


/// Tag indicating that the base level construct is virtual
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like member functions or class inheritance and
 *  indicates that the member function is virtual or the base
 *  class in question is inherited virtually.
 *
 *  @ingroup specifier_tags
 */
struct spec_virtual_tag
 : spec_inheritance_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("virtual", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_virtual(MACRO, DATA, SEP_MACRO) \
	MACRO(inheritance_type, DATA)


/// Tag indicating that the base level construct is non-virtual
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like member functions or class inheritance and
 *  indicates that the member function is non-virtual or the base
 *  class in question is not inherited virtually.
 *
 *  @ingroup specifier_tags
 */
struct spec_non_virtual_tag
 : spec_inheritance_type_tag
 , spec__tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_non_virtual(MACRO, DATA, SEP_MACRO) \
	MACRO(inheritance_type, DATA) \
	SEP_MACRO() \
	MACRO(none, DATA)


/// Common base for storage class or linkage tags
/**
 *  @ingroup specifier_tags
 */
struct spec_storage_class_tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_storage_class(MACRO, DATA, SEP_MACRO)
#define MIRROR_FOR_EACH_SPEC_TYPE_storage_class(MACRO, DATA, SEP_MACRO) \
	MACRO(static, DATA) \
	SEP_MACRO() \
	MACRO(mutable, DATA) \
	SEP_MACRO() \
	MACRO(auto, DATA) \
	SEP_MACRO() \
	MACRO(extern, DATA) \
	SEP_MACRO() \
	MACRO(register, DATA) \
	SEP_MACRO() \
	MACRO(thread_local, DATA)


/// Tag indicating that the base level construct has static storage class
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class member variables or functions and
 *  indicates that the construct has static storage class.
 *
 *  @ingroup specifier_tags
 */
struct spec_static_tag
 : spec_storage_class_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("static", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_static(MACRO, DATA, SEP_MACRO) \
	MACRO(storage_class, DATA)


/// Tag indicating that the base level construct has mutable storage class
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class member variables or functions and
 *  indicates that the construct has mutable storage class.
 *
 *  @ingroup specifier_tags
 */
struct spec_mutable_tag
 : spec_storage_class_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("mutable", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_mutable(MACRO, DATA, SEP_MACRO) \
	MACRO(storage_class, DATA)


/// Tag indicating that the base level construct has automatic storage class
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class member variables or functions and
 *  indicates that the construct has automatic storage class.
 *
 *  @ingroup specifier_tags
 */
struct spec_auto_tag
 : spec_storage_class_tag
 , spec__tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_auto(MACRO, DATA, SEP_MACRO) \
	MACRO(storage_class, DATA) \
	SEP_MACRO() \
	MACRO(none, DATA)


/// Tag indicating that the base level construct has extern storage class
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like global variables, etc. and indicates that
 *  the construct has extern storage class.
 *
 *  @ingroup specifier_tags
 */
struct spec_extern_tag
 : spec_storage_class_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("extern", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_extern(MACRO, DATA, SEP_MACRO) \
	MACRO(storage_class, DATA)


/// Tag indicating that the base level construct has register storage class
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like global variables, etc. and indicates that
 *  the construct has register storage class.
 *
 *  @ingroup specifier_tags
 */
struct spec_register_tag
 : spec_storage_class_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("register", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_register(MACRO, DATA, SEP_MACRO) \
	MACRO(storage_class, DATA)


/// Tag indicating that the base level construct has thread_local storage class
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like global variables, members, etc. and indicates that
 *  the construct has thread_local storage class.
 *
 *  @ingroup specifier_tags
 */
struct spec_thread_local_tag
 : spec_storage_class_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("thread_local", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_thread_local(MACRO, DATA, SEP_MACRO) \
	MACRO(storage_class, DATA)


/// Common base for constness tags
/**
 *  @ingroup specifier_tags
 */
struct spec_constness_tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_constness(MACRO, DATA, SEP_MACRO)
#define MIRROR_FOR_EACH_SPEC_TYPE_constness(MACRO, DATA, SEP_MACRO) \
	MACRO(const, DATA) \
	SEP_MACRO() \
	MACRO(non_const, DATA)

/// Tag indicating that the base level construct is const
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class' member functions and indicates that
 *  the function is declared as const.
 *
 *  @ingroup specifier_tags
 */
struct spec_const_tag
 : spec_constness_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("const", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_const(MACRO, DATA, SEP_MACRO) \
	MACRO(constness, DATA)


/// Tag indicating that the base level construct is not const
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class' member functions and indicates that
 *  the function is declared as non-const.
 *
 *  @ingroup specifier_tags
 */
struct spec_non_const_tag
 : spec_constness_tag
 , spec__tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_non_const(MACRO, DATA, SEP_MACRO) \
	MACRO(constness, DATA) \
	SEP_MACRO() \
	MACRO(none, DATA)


/// Common base for access type tags
/**
 *  @ingroup specifier_tags
 */
struct spec_access_type_tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_access_type(MACRO, DATA, SEP_MACRO)
#define MIRROR_FOR_EACH_SPEC_TYPE_access_type(MACRO, DATA, SEP_MACRO) \
	MACRO(private, DATA) \
	SEP_MACRO() \
	MACRO(protected, DATA) \
	SEP_MACRO() \
	MACRO(public, DATA)


/// Tag indicating that the base level construct has private access
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class' members or base classes and indicates that
 *  the member or base class has private access specifier.
 *
 *  @ingroup specifier_tags
 */
struct spec_private_tag
 : spec_access_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("private", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_private(MACRO, DATA, SEP_MACRO) \
	MACRO(access_type, DATA)


/// Tag indicating that the base level construct has protected access
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class' members or base classes and indicates that
 *  the member or base class has protected access specifier.
 *
 *  @ingroup specifier_tags
 */
struct spec_protected_tag
 : spec_access_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("protected", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_protected(MACRO, DATA, SEP_MACRO) \
	MACRO(access_type, DATA)


/// Tag indicating that the base level construct has public access
/** This tag is used by compile-time meta-objects reflecting
 *  constructs like class' members or base classes and indicates that
 *  the member or base class has public access specifier.
 *
 *  @ingroup specifier_tags
 */
struct spec_public_tag
 : spec_access_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("public", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_public(MACRO, DATA, SEP_MACRO) \
	MACRO(access_type, DATA)

/// Common base for elaborated type tags
/**
 *  @ingroup specifier_tags
 */
struct spec_elaborated_type_tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_elaborated_type(MACRO, DATA, SEP_MACRO)
#define MIRROR_FOR_EACH_SPEC_TYPE_elaborated_type(MACRO, DATA, SEP_MACRO) \
	MACRO(struct, DATA) \
	SEP_MACRO() \
	MACRO(class, DATA) \
	SEP_MACRO() \
	MACRO(union, DATA) \
	SEP_MACRO() \
	MACRO(enum, DATA) \
	SEP_MACRO() \
	MACRO(type, DATA)


/// Tag indicating that the reflected elaborated type is declared as 'struct'
/** This tag is used by meta_classes to indicate the reflected class
 *  has been defined (and registered) as 'struct'
 *
 *  @ingroup specifier_tags
 */
struct spec_struct_tag
 : spec_elaborated_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("struct", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_struct(MACRO, DATA, SEP_MACRO) \
	MACRO(elaborated_type, DATA)

/// Tag indicating that the reflected elaborated type is declared as 'class'
/** This tag is used by meta_classes to indicate the reflected type
 *  has been defined (and registered) as 'class' (as opposed to 'struct'
 *  or 'union').
 *
 *  @ingroup specifier_tags
 */
struct spec_class_tag
 : spec_elaborated_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("class", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_class(MACRO, DATA, SEP_MACRO) \
	MACRO(elaborated_type, DATA)

/// Tag indicating that the reflected elaborated type is declared as 'union'
/** This tag is used by meta_classes to indicate the reflected type
 *  has been defined (and registered) as 'union'
 *
 *  @ingroup specifier_tags
 */
struct spec_union_tag
 : spec_elaborated_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("union", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_union(MACRO, DATA, SEP_MACRO) \
	MACRO(elaborated_type, DATA)

/// Tag indicating that the reflected elaborated type is declared as 'enum'
/** This tag is used by meta_classes to indicate the reflected type
 *  has been defined (and registered) as 'enum'
 *
 *  @ingroup specifier_tags
 */
struct spec_enum_tag
 : spec_elaborated_type_tag
{MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS("enum", __LINE__)};

#define MIRROR_FOR_EACH_SPEC_BASE_enum(MACRO, DATA, SEP_MACRO) \
	MACRO(elaborated_type, DATA)

/// Tag indicating that the reflected type is a non-elaborated type
/** This tag is used by meta_classes to indicate the reflected type
 *  has been defined (and registered) as non-elaborated type.
 *  This specifier is reserved for the intrinsic C++ types and should
 *  NOT be used in client code.
 *
 *  @ingroup specifier_tags
 */
struct spec_type_tag
 : spec_elaborated_type_tag
 , spec__tag { };

#define MIRROR_FOR_EACH_SPEC_BASE_type(MACRO, DATA, SEP_MACRO) \
	MACRO(elaborated_type, DATA) \
	SEP_MACRO() \
	MACRO(none, DATA)


/// Special specifier tag for selecting the default specifier in a context
/** This specifier tag is used as a convenience for selecting the default
 *  specifier for the particular context in which it is used.
 *  For example if this specifier is used to specify the inheritance type
 *  then the non-virtual inheritance is selected, if it is used to specify
 *  the default 'staticity' or 'constness' of a class member function
 *  then in the first case non-static tag and in the second cast the
 *  non-const tag is selected.
 *
 *  Do not use this tag directly, it is used by some of the registering
 *  macros in an appropriate way.
 *
 *  @ingroup specifier_tags
 */
struct spec___tag { };

// undefine the helper macros
#undef MIRROR_IMPLEMENT_SPECIFIER_TAG_KEYWORD_FUNCTIONS

#define MIRROR_FOR_EACH_SPECIFIER(MACRO, DATA) \
	MACRO(none,	     false, false,  1, DATA) \
	MACRO(inheritance_type, false,  true,  2, DATA) \
	MACRO(non_virtual,      false, false,  3, DATA) \
	MACRO(virtual,	   true, false,  4, DATA) \
	MACRO(storage_class,    false,  true,  5, DATA) \
	MACRO(auto,	     false, false,  6, DATA) \
	MACRO(extern,	    true, false,  7, DATA) \
	MACRO(static,	    true, false,  8, DATA) \
	MACRO(mutable,	   true, false,  9, DATA) \
	MACRO(register,	  true, false, 10, DATA) \
	MACRO(thread_local,      true, false, 11, DATA) \
	MACRO(constness,	false,  true, 12, DATA) \
	MACRO(non_const,	false, false, 13, DATA) \
	MACRO(const,	     true, false, 14, DATA) \
	MACRO(access_type,      false,  true, 15, DATA) \
	MACRO(private,	   true, false, 16, DATA) \
	MACRO(protected,	 true, false, 17, DATA) \
	MACRO(public,	    true, false, 18, DATA) \
	MACRO(elaborated_type,  false,  true, 19, DATA) \
	MACRO(type,	     false, false, 20, DATA) \
	MACRO(class,	     true, false, 21, DATA) \
	MACRO(struct,	    true, false, 22, DATA) \
	MACRO(union,	     true, false, 23, DATA) \
	MACRO(enum,	      true, false, 24, DATA)

#define MIRROR_FOR_EACH_SPECIFIER_BASE(SPEC, MACRO, DATA, SEP_MACRO) \
	MIRROR_FOR_EACH_SPEC_BASE_##SPEC(MACRO, DATA, SEP_MACRO)

#define MIRROR_FOR_EACH_SPECIFIER_TYPE(SPEC, MACRO, DATA, SEP_MACRO) \
	MIRROR_FOR_EACH_SPEC_TYPE_##SPEC(MACRO, DATA, SEP_MACRO)


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function template returning the ElaboratedTypeTag for a type
/** This template can be used to determine how the type passed as
 *  template argument has been registered (as plain type, as enum,
 *  as class, etc.)
 */
template <typename T>
struct type_category
{
	typedef ElaboratedTypeTag type;
};

/// Function returning the ElaboratedTypeTag for the type of its argument
template <typename T>
ElaboratedTypeTag categorize_type(const T&);
#endif

#define MIRROR_HLPR_MAKE_SPEC_TAG_LISTS(SPEC, X) spec_ ## SPEC ## _tag

/// Returns a range of all inheritance type specifier tags
/**
 *  @see InheritanceTypeTag
 *
 *  @ingroup specifier_tags
 */
struct inheritance_type_tags
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	typedef Range<InheritanceTypeTag> type;
#else
	typedef mp::range<
		MIRROR_FOR_EACH_SPECIFIER_TYPE(
			inheritance_type,
			MIRROR_HLPR_MAKE_SPEC_TAG_LISTS,
			_,
			MIRROR_PP_COMMA
		)
	> type;
#endif
};

/// Returns a range of all storage class specifier tags
/**
 *  @see StorageClassTag
 *
 *  @ingroup specifier_tags
 */
struct storage_class_tags
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	typedef Range<StorageClassTag> type;
#else
	typedef mp::range<
		MIRROR_FOR_EACH_SPECIFIER_TYPE(
			storage_class,
			MIRROR_HLPR_MAKE_SPEC_TAG_LISTS,
			_,
			MIRROR_PP_COMMA
		)
	> type;
#endif
};

/// Returns a range of all constness specifier tags
/**
 *  @see ConstnessTag
 *
 *  @ingroup specifier_tags
 */
struct constness_tags
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	typedef Range<ConstnessTag> type;
#else
	typedef mp::range<
		MIRROR_FOR_EACH_SPECIFIER_TYPE(
			constness,
			MIRROR_HLPR_MAKE_SPEC_TAG_LISTS,
			_,
			MIRROR_PP_COMMA
		)
	> type;
#endif
};

/// Returns a range of all access type specifier tags
/**
 *  @see AccessTypeTag
 *
 *  @ingroup specifier_tags
 */
struct access_type_tags
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	typedef Range<AccessTypeTag> type;
#else
	typedef mp::range<
		MIRROR_FOR_EACH_SPECIFIER_TYPE(
			access_type,
			MIRROR_HLPR_MAKE_SPEC_TAG_LISTS,
			_,
			MIRROR_PP_COMMA
		)
	> type;
#endif
};


/// Returns a range of all elaborated type specifier tags
/**
 *  @see ElaboratedTypeTag
 *
 *  @ingroup specifier_tags
 */
struct elaborated_type_tags
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	typedef Range<ElaboratedTypeTag> type;
#else
	typedef mp::range<
		MIRROR_FOR_EACH_SPECIFIER_TYPE(
			elaborated_type,
			MIRROR_HLPR_MAKE_SPEC_TAG_LISTS,
			_,
			MIRROR_PP_COMMA
		)
	> type;
#endif
};

#undef MIRROR_HLPR_MAKE_SPEC_TAG_LISTS

MIRROR_NAMESPACE_END

#endif //include guard

