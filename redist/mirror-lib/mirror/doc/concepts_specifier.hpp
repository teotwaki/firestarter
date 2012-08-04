/**
 *  @file mirror/doc/concepts_specifier.hpp
 *  @brief Documentation of tag classes which reflect various base-level
 *  construct specifiers for storage class, accessibility, etc.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_SPECIFIER_1011291729_HPP
#define MIRROR_DOC_CONCEPTS_SPECIFIER_1011291729_HPP

#include <mirror/config.hpp>

#ifdef MIRROR_DOCUMENTATION_ONLY

MIRROR_NAMESPACE_BEGIN
// This section is here for documentation purposes only

/** @defgroup specifier_concepts Mirror - Specifiers concepts
 *
 *  These are models for tag classes which reflect various base-level
 *  construct specifiers for storage class, class' member accessibility,
 *  inheritance types, etc.
 *  These concept classes are defined here only for documentation purposes
 *  and cannot be used in any end-user code.
 */

/// SpecifierTag is a class reflecting a trait specified by a specifier keyword
/** SpecifierTag(s) can be used to examine various traits of base-level
 *  constructs  like base class inheritance type, member function constness,
 *  variable storage class, etc. which are specified by various specifier
 *  keywords, like virtual, public, protected, private, mutable, static, etc.
 *
 *  @see InheritanceTypeTag
 *  @see StorageClassTag
 *  @see ConstnessTag
 *  @see AccessTypeTag
 *  @see ElaboratedTypeTag
 *
 *  @ingroup specifier_concepts
 */
struct SpecifierTag
{
	/// Returnins the keyword for the specifier represented by this tag
	/** This function can be used to get the keyword for the specifier
	 *  which is represented by the SpecifierTag type.
	 */
	static std::string keyword(void);
};

/// InheritanceTypeTag is a type which reflects the type of class inheritance
/**
 *  There are two types which are models of this concept - spec_virtual_tag
 *  which represents the virtual inheritance type and spec_non_virtual_tag
 *  which represents non-virtual class inheritance.
 *
 *  @see spec_virtual_tag
 *  @see spec_non_virtual_tag
 *
 *  @see inheritance_type_tags
 *
 *  @ingroup specifier_concepts
 */
struct InheritanceTypeTag
 : virtual public SpecifierTag
{ };

/// StorageClassTag is a type which reflects the storage class of a construct
/**
 *  There are several types which are models of this concept - spec_auto_tag
 *  which represents automatic storage class, spec_extern_tag representing
 *  extern storage class, spec_static_tag for static storage class,
 *  spec_mutable_tag for mutable storage class, spec_register_tag representing
 *  the register storage class and spec_thread_local_tag representing
 *  thread-local storage class.
 *
 *  @see spec_auto_tag
 *  @see spec_extern_tag
 *  @see spec_static_tag
 *  @see spec_mutable_tag
 *  @see spec_register_tag
 *  @see spec_thread_local_tag
 *
 *  @see storage_class_tags
 *
 *  @ingroup specifier_concepts
 */
struct StorageClassTag
 : virtual public SpecifierTag
{ };

/// ConstnessTag is a type that reflects the member function constness
/**
 *  There are two types which are models of this concept - spec_const_tag
 *  representing const member functions and spec_non_const_tag for non-const
 *  member functions of a class.
 *
 *  @see spec_const_tag
 *  @see spec_non_const_tag
 *
 *  @see constness_tags
 *
 *  @ingroup specifier_concepts
 */
struct ConstnessTag
 : virtual public SpecifierTag
{ };

/// AccessTypeTag is a type that reflects the class member access type specifier
/**
 *  There are three types which are models of this concept - spec_private_tag
 *  representing private access type, spec_protected_tag for protected
 *  member access type and spec_public_tag for public access type.
 *
 *  @see spec_private_tag
 *  @see spec_protected_tag
 *  @see spec_public_tag
 *
 *  @see access_type_tags
 *
 *  @ingroup specifier_concepts
 */
struct AccessTypeTag
 : virtual public SpecifierTag
{ };

/// ElaboratedTypeTag is a type that reflects the elaborated type specifier
/**
 *  There are several types which are models of this concept - spec_class_tag
 *  representing elaborated user types defines with the 'class' specifier,
 *  spec_struct_tag for user types defined as struct(s), spec_union_tag for
 *  union(s) and spec_enum_tag for enum(s).
 *
 *  @see spec_class_tag
 *  @see spec_struct_tag
 *  @see spec_union_tag
 *  @see spec_enum_tag
 *  @see spec_type_tag
 *
 *  @see elaborated_type_tags
 *
 *  @ingroup specifier_concepts
 */
struct ElaboratedTypeTag
 : virtual public SpecifierTag
{ };

MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

