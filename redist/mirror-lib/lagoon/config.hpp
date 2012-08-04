/**
 * @file lagoon/config.hpp
 * @brief Compilation configuration options
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_CONFIG_1011291729_HPP
#define LAGOON_CONFIG_1011291729_HPP

// Macro that begins the Lagoon library namespace
#define LAGOON_NAMESPACE_BEGIN \
namespace lagoon {

// Macro that ends the Lagoon library namespace
#define LAGOON_NAMESPACE_END \
} /* namespace lagoon */

/** @defgroup lagoon_config_options Lagoon - Configuration options
 *
 *  The polymorphic nature of the interfaces and their implementations
 *  makes Lagoon-using applications prone to have long compilation times
 *  and large resulting executables.
 *  Lagoon is however highly configurable via preprocessor options,
 *  and lets the user preciselly specify which features should
 *  be included in the interfaces and what should be ommited. This
 *  way the application does not pay for things it does not use
 *  in terms of resources necessary for the compilation and the resulting
 *  code size.
 */

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_DYNAMIC_LINKING
 *  @brief Enables the dynamic linking of Lagoon's meta-objects
 *
 *  Setting this symbol to a zero vs. nonzero integer value switches
 *  between the header-only statically linked and the dynamically linked
 *  meta-objects respectivelly.
 *
 *  TODO: elaborate on what's the difference
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_DYNAMIC_LINKING 0 // or 1
#endif
#ifndef LAGOON_DYNAMIC_LINKING
#define LAGOON_DYNAMIC_LINKING 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MMO_WITH_GENERALIZATIONS
 *  @brief Enables the meta_meta_type's generalizations member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_meta_type::generalizations generalizations@endlink
 *  member function in the meta_meta_type interface.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MMO_WITH_GENERALIZATIONS 0 // or 1
#endif
#ifndef LAGOON_MMO_WITH_GENERALIZATIONS
#define LAGOON_MMO_WITH_GENERALIZATIONS 0
#endif

#define LAGOON_POLYMORPHIC_MMO (LAGOON_MMO_WITH_GENERALIZATIONS)

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MNSO_WITH_LOCAL_NAME
 *  @brief Enables the meta_named_scoped_objects's local_name member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_named_scoped_object::local_name local_name@endlink
 *  member function in the meta_named_scoped_object interface.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MNSO_WITH_LOCAL_NAME 0 // or 1
#else
#ifndef LAGOON_MNSO_WITH_LOCAL_NAME
#define LAGOON_MNSO_WITH_LOCAL_NAME 0
#endif
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_ALL_TYPE_TRAITS
 *  @brief Enables the type trait-related member function in the meta_type
 *
 *  Setting this to a non-zero value enables all supported
 *  type traits member functions in the meta_type interface.
 *  Otherwise the individual traits can be enabled separatelly
 *  by setting the appropriate preprocessor switches (listed below).
 *
 *  Set to zero (disabled) by default.
 *
 *  @see LAGOON_MT_WITH_DEFAULT
 *  @see LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE
 *  @see LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE
 *  @see LAGOON_MT_WITH_ALIGNMENT_OF
 *  @see LAGOON_MT_WITH_IS_ARRAY
 *  @see LAGOON_MT_WITH_IS_CONST
 *  @see LAGOON_MT_WITH_IS_VOLATILE
 *  @see LAGOON_MT_WITH_IS_POINTER
 *  @see LAGOON_MT_WITH_IS_REFERENCE
 *  @see LAGOON_MT_WITH_ADD_CONST
 *  @see LAGOON_MT_WITH_ADD_CV
 *  @see LAGOON_MT_WITH_ADD_VOLATILE
 *  @see LAGOON_MT_WITH_REMOVE_CONST
 *  @see LAGOON_MT_WITH_REMOVE_CV
 *  @see LAGOON_MT_WITH_REMOVE_VOLATILE
 *  @see LAGOON_MT_WITH_REMOVE_POINTER
 *  @see LAGOON_MT_WITH_REMOVE_REFERENCE
 *  @see LAGOON_MT_WITH_REMOVE_EXTENT
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_ALL_TYPE_TRAITS 0 // or 1
#endif
#ifndef LAGOON_ALL_TYPE_TRAITS
#define LAGOON_ALL_TYPE_TRAITS 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_DEFAULT
 *  @brief Enables the meta_type's default_ member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::default_ default_@endlink
 *  member function in the meta_type interface.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_DEFAULT 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_DEFAULT
#define LAGOON_MT_WITH_DEFAULT 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE
 *  @brief Enables the meta_type's is_default_constructible member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_default_constructible
 *  is_default_constructible@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE
#define LAGOON_MT_WITH_IS_DEFAULT_CONSTRUCTIBLE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE
 *  @brief Enables the meta_type's is_copy_constructible member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_copy_constructible
 *  is_copy_constructible@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE
#define LAGOON_MT_WITH_IS_COPY_CONSTRUCTIBLE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_ALIGNMENT_OF
 *  @brief Enables the meta_type's alignment_of member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::alignment_of alignment_of@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_ALIGNMENT_OF 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_ALIGNMENT_OF
#define LAGOON_MT_WITH_ALIGNMENT_OF 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_ARRAY
 *  @brief Enables the meta_type's is_array member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_array is_array@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_ARRAY 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_ARRAY
#define LAGOON_MT_WITH_IS_ARRAY 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_CONST
 *  @brief Enables the meta_type's is_const member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_const is_const@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_CONST 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_CONST
#define LAGOON_MT_WITH_IS_CONST 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_POINTER
 *  @brief Enables the meta_type's is_pointer member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_pointer is_pointer@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_POINTER 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_POINTER
#define LAGOON_MT_WITH_IS_POINTER 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_REFERENCE
 *  @brief Enables the meta_type's is_reference member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_reference is_reference@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_REFERENCE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_REFERENCE
#define LAGOON_MT_WITH_IS_REFERENCE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_IS_VOLATILE
 *  @brief Enables the meta_type's is_volatile member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::is_volatile is_volatile@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_IS_VOLATILE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_IS_VOLATILE
#define LAGOON_MT_WITH_IS_VOLATILE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_ADD_CONST
 *  @brief Enables the meta_type's add_const member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::add_const add_const@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_ADD_CONST 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_ADD_CONST
#define LAGOON_MT_WITH_ADD_CONST 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_ADD_CV
 *  @brief Enables the meta_type's add_cv member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::add_cv add_cv@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_ADD_CV 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_ADD_CV
#define LAGOON_MT_WITH_ADD_CV 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_ADD_VOLATILE
 *  @brief Enables the meta_type's add_volatile member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::add_volatile add_volatile@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_ADD_VOLATILE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_ADD_VOLATILE
#define LAGOON_MT_WITH_ADD_VOLATILE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_REMOVE_CONST
 *  @brief Enables the meta_type's remove_const member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::remove_const remove_const@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_REMOVE_CONST 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_REMOVE_CONST
#define LAGOON_MT_WITH_REMOVE_CONST 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_REMOVE_CV
 *  @brief Enables the meta_type's remove_cv member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::remove_cv remove_cv@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_REMOVE_CV 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_REMOVE_CV
#define LAGOON_MT_WITH_REMOVE_CV 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_REMOVE_VOLATILE
 *  @brief Enables the meta_type's remove_volatile member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::remove_volatile remove_volatile@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_REMOVE_VOLATILE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_REMOVE_VOLATILE
#define LAGOON_MT_WITH_REMOVE_VOLATILE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_REMOVE_POINTER
 *  @brief Enables the meta_type's remove_pointer member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::remove_pointer remove_pointer@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_REMOVE_POINTER 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_REMOVE_POINTER
#define LAGOON_MT_WITH_REMOVE_POINTER 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_REMOVE_REFERENCE
 *  @brief Enables the meta_type's remove_reference member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::remove_reference remove_reference@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_REMOVE_REFERENCE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_REMOVE_REFERENCE
#define LAGOON_MT_WITH_REMOVE_REFERENCE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_REMOVE_EXTENT
 *  @brief Enables the meta_type's remove_extent member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::remove_extent remove_extent@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_ALL_TYPE_TRAITS
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_REMOVE_EXTENT 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_REMOVE_EXTENT
#define LAGOON_MT_WITH_REMOVE_EXTENT 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_MAKE_FACTORY
 *  @brief Enables the meta_type's make_factory member function.
 *
 *  Setting this switch to a non-zero value enables the meta_type's
 *  @link lagoon::meta_type::make_factory make_factory@endlink
 *  member function, which can be used to build
 *  factories creating instances of the type reflected by meta-type.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_MAKE_FACTORY 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_MAKE_FACTORY
#define LAGOON_MT_WITH_MAKE_FACTORY 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_NEW
 *  @brief Enables the meta_type's new_ member function.
 *
 *  Setting this to non-zero integral value enables the
 *  @link lagoon::meta_type::new_ new_@endlink member
 *  function in the meta_type interface.
 *
 *  @see LAGOON_MT_WITH_NEW_COPY
 *  @see LAGOON_MT_WITH_DELETE
 *  @see LAGOON_MT_WITH_MAKE_SHARED
 *  @see LAGOON_MT_WITH_MAKE_UNIQUE
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_NEW 1 // or 0
#endif
#ifndef LAGOON_MT_WITH_NEW
#define LAGOON_MT_WITH_NEW 1
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_NEW_COPY
 *  @brief Enables the meta_type's new_copy member function.
 *
 *  Setting this to non-zero integral value enables the
 *  @link lagoon::meta_type::new_copy new_copy@endlink member
 *  function in the meta_type interface.
 *
 *  @see LAGOON_MT_WITH_NEW
 *  @see LAGOON_MT_WITH_DELETE
 *  @see LAGOON_MT_WITH_MAKE_SHARED
 *  @see LAGOON_MT_WITH_MAKE_UNIQUE
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_NEW_COPY 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_NEW_COPY
#define LAGOON_MT_WITH_NEW_COPY 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_DELETE
 *  @brief Enables the meta_type's delete_ member function.
 *
 *  Setting this to non-zero integral value enables the
 *  @link lagoon::meta_type::delete_ delete_@endlink member
 *  function in the meta_type interface.
 *
 *  @see LAGOON_MT_WITH_NEW
 *  @see LAGOON_MT_WITH_NEW_COPY
 *  @see LAGOON_MT_WITH_MAKE_SHARED
 *  @see LAGOON_MT_WITH_MAKE_UNIQUE
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_DELETE 1 // or 0
#endif
#ifndef LAGOON_MT_WITH_DELETE
#define LAGOON_MT_WITH_DELETE 1
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_MAKE_SHARED
 *  @brief Enables the meta_type's make_shared member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::make_shared make_shared@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_MT_WITH_MAKE_SHARED
 *  @see LAGOON_MT_WITH_DELETE
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_MAKE_SHARED 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_MAKE_SHARED
#define LAGOON_MT_WITH_MAKE_SHARED 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MT_WITH_MAKE_UNIQUE
 *  @brief Enables the meta_type's make_unique member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_type::make_unique make_unique@endlink
 *  member function in the meta_type interface.
 *
 *  @see LAGOON_MT_WITH_MAKE_SHARED
 *  @see LAGOON_MT_WITH_DELETE
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MT_WITH_MAKE_UNIQUE 0 // or 1
#endif
#ifndef LAGOON_MT_WITH_MAKE_UNIQUE
#define LAGOON_MT_WITH_MAKE_UNIQUE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_MC_WITH_ALL_MEMBER_VARIABLES
 *  @brief Enables the meta_class' all_member_variables member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::meta_class::all_member_variables
 *  all_member_variables@endlink
 *  member function in the meta_class interface.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_MC_WITH_ALL_MEMBER_VARIABLES 0 // or 1
#endif
#ifndef LAGOON_MC_WITH_ALL_MEMBER_VARIABLES
#define LAGOON_MC_WITH_ALL_MEMBER_VARIABLES 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_FACT_WITH_CREATE
 *  @brief Enables the polymorph_factory's create member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link lagoon::polymorph_factory::create create@endlink
 *  member function in the polymorphic_factory interface.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_FACT_WITH_CREATE 0 // or 1
#endif
#ifndef LAGOON_FACT_WITH_CREATE
#define LAGOON_FACT_WITH_CREATE 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_NO_NAMESPACE_MEMBERS
 *  @brief Disables the traversal of namespace members
 *
 *  If this is defined as zero then namespace members can be iterated,
 *  otherwise the members() function for meta-namespace
 *  returns an empty range.
 *
 *  Setting this to a non-zero value can greatly improve compilation
 *  times if namespace member traversal is not needed.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_NO_NAMESPACE_MEMBERS 0 // or 1
#endif
#ifndef LAGOON_NO_NAMESPACE_MEMBERS
#define LAGOON_NO_NAMESPACE_MEMBERS 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def LAGOON_NO_ENUM_MEMBERS
 *  @brief Disables the enum values traversal via members()
 *
 *  If this is defined as zero then enumeration value can be iterated.
 *  Otherwise the members() function for meta-enum
 *  returns an empty range.
 *  This, however does not disable the meta-enum's size, value,
 *  value_by_name, name_by_value, etc. member functions.
 *
 *  Setting this to a non-zero value can improve compilation
 *  times if this manner of enum value traversal is not needed.
 *
 *  @ingroup lagoon_config_options
 */
#define LAGOON_NO_ENUM_MEMBERS 0 // or 1
#endif
#ifndef LAGOON_NO_ENUM_MEMBERS
#define LAGOON_NO_ENUM_MEMBERS 0
#endif

#endif //include guard

