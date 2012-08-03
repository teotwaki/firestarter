/**
 *  @file mirror/auxiliary/default_spec_tags.hpp
 *  @brief Internal translation from the default specifier tags to the
 *  specifier tag appropriate to a particular context.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_DEFAULT_SPEC_TAGS_1011291729_HPP
#define MIRROR_AUX_DEFAULT_SPEC_TAGS_1011291729_HPP

#include <mirror/specifier_tags.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Meta-function which returns the inheritance type (virtual|non-virtual)
/* This meta-function template class can be used for checking if the
 * passed Tag is a valid class inheritance type tag and to get the default
 * inheritance type tag if the special default tag is used.
 *
 * This template has specializations only for spec_virtual_tag,
 * spec_non_virtual_tag and spec___tag. Instantiation failure indicates
 * that an invalid tag was passed as the template argument.
 */
template <typename Tag>
struct inheritance_type_tag;

// specialization for 'virtual' specifier tag
template <>
struct inheritance_type_tag<spec_virtual_tag>
{
	typedef spec_virtual_tag type;
};

// specialization for '' (non-virtual) specifier tag
template <>
struct inheritance_type_tag<spec_non_virtual_tag>
{
	typedef spec_non_virtual_tag type;
};

// specialization for the default specifier (returns non-virtual)
template <>
struct inheritance_type_tag<spec___tag>
{
	typedef spec_non_virtual_tag type;
};


// Meta-function which returns the member access (public|protected|private)
/* This meta-function template class can be used to check if the passed
 * Tag is a valid class' member access type tag and to get the default
 * access type for the particular ElaboratedType, if the special default tag
 * is used.
 * For example if the ElaboratedType is 'class' then the default access is private
 * for 'struct'(s) the default access is public.
 *
 * This template has specializations for spec_public_tag, spec_protected_tag
 * spec_private_tag and spec___tag. Instantiation failure indicates
 * that an invalid access type tag has been passed as the template argument.
 */
template <typename ElaboratedType, typename AccessTag>
struct access_type_tag;

// Specialization for public access
template <typename ElaboratedType>
struct access_type_tag<ElaboratedType, spec_public_tag>
{
	typedef spec_public_tag type;
};

// Specialization for protected access
template <typename ElaboratedType>
struct access_type_tag<ElaboratedType, spec_protected_tag>
{
	typedef spec_protected_tag type;
};

// Specialization for private access
template <typename ElaboratedType>
struct access_type_tag<ElaboratedType, spec_private_tag>
{
	typedef spec_private_tag type;
};

// Default access specifier for classes
template <>
struct access_type_tag<spec_class_tag, spec___tag>
{
	typedef spec_private_tag type;
};

// Default access specifier for structs
template <>
struct access_type_tag<spec_struct_tag, spec___tag>
{
	typedef spec_public_tag type;
};

// Default access specifier for enums
template <>
struct access_type_tag<spec_enum_tag, spec___tag>
{
	typedef spec_public_tag type;
};

// Meta-function which returns the tag indicating constness of a mem. function
/* This meta-function template class can be used to check if the passed
 * Tag is a valid class member's constness tag and to get the default constness
 * indicator if the special default tag is used.
 *
 * This template has specializations for spec_const_tag and spec_non_const_tag.
 * Instantiation failure indicates that an invalid tag has been passed as
 * the template argument.
 */
template <typename Tag>
struct constness_tag;

// Specialization for 'const' member function specifier
template <>
struct constness_tag<spec_const_tag>
{
	typedef spec_const_tag type;
};

// Specialization for non-const member function specifier
template <>
struct constness_tag<spec_non_const_tag>
{
	typedef spec_non_const_tag type;
};

// Specialization for non-const member functions
template <>
struct constness_tag<spec___tag>
{
	typedef spec_non_const_tag type;
};

// Meta-function which returns the tag indicating storage class of a construct
/* This meta-function template class can be used to check if the passed
 * Tag is a valid storage class specification tag and to get the default
 * storage class specifier if the special default tag is used.
 *
 * This template has specializations for spec_auto_tag, spec_static_tag,
 * spec_extern_tag, spec_mutable_tag, spec_register_tag, spec_thread_local_tag,
 * spec___tag.
 * Instantiation failure indicates that an invalid tag has been passed
 * as the template argument.
 */
template <typename Tag>
struct storage_class_tag;

// Specialization for automatic (the default) storage class tag
template <>
struct storage_class_tag<spec_auto_tag>
{
	typedef spec_auto_tag type;
};

// Specialization for static storage class tag
template <>
struct storage_class_tag<spec_static_tag>
{
	typedef spec_static_tag type;
};

// Specialization for extern storage class tag
template <>
struct storage_class_tag<spec_extern_tag>
{
	typedef spec_extern_tag type;
};

// Specialization for mutable storage class tag
template <>
struct storage_class_tag<spec_mutable_tag>
{
	typedef spec_mutable_tag type;
};

// Specialization for register storage class tag
template <>
struct storage_class_tag<spec_register_tag>
{
	typedef spec_register_tag type;
};

// Specialization for thread local storage class tag
template <>
struct storage_class_tag<spec_thread_local_tag>
{
	typedef spec_thread_local_tag type;
};

// free variables
template <typename Tag>
struct storage_class_tag_fv : storage_class_tag<Tag> { };

template <>
struct storage_class_tag_fv<spec___tag>
{
	typedef spec_static_tag type;
};

// local variables
template <typename Tag>
struct storage_class_tag_lv : storage_class_tag<Tag> { };

template <>
struct storage_class_tag_lv<spec___tag>
{
	typedef spec_auto_tag type;
};

// member variables
template <typename Tag>
struct storage_class_tag_mv : storage_class_tag<Tag> { };

template <>
struct storage_class_tag_mv<spec___tag>
{
	typedef spec_auto_tag type;
};

// free functions
template <typename Tag>
struct storage_class_tag_ff : storage_class_tag<Tag> { };

template <>
struct storage_class_tag_ff<spec___tag>
{
	typedef spec_static_tag type;
};

// member functions
template <typename Tag>
struct storage_class_tag_mf : storage_class_tag<Tag> { };

template <>
struct storage_class_tag_mf<spec___tag>
{
	typedef spec_auto_tag type;
};


// Meta-function which returns the tag specifying elaborate type kind
/* This meta-function template class can be used to check if the passed
 * Tag is a valid elaborate type specification tag and to get the default
 * elaborate type specifier if the special default tag is used.
 *
 * This template has specializations for spec_class_tag, spec_struct_tag,
 * spec_union_tag, spec_enum_tag, spec___tag.
 * Instantiation failure indicates that an invalid tag has been passed
 * as the template argument.
 */
template <typename Tag>
struct elaborated_type_tag;

// Specialization for the class elaborate type tag
template <>
struct elaborated_type_tag<spec_class_tag>
{
	typedef spec_class_tag type;
};

// Specialization for the struct elaborate type tag
template <>
struct elaborated_type_tag<spec_struct_tag>
{
	typedef spec_struct_tag type;
};

// Specialization for the union elaborate type tag
template <>
struct elaborated_type_tag<spec_union_tag>
{
	typedef spec_union_tag type;
};

// Specialization for the enum elaborate type tag
template <>
struct elaborated_type_tag<spec_enum_tag>
{
	typedef spec_enum_tag type;
};

// Specialization for the non-elaborate type tag
template <>
struct elaborated_type_tag<spec_type_tag>
{
	typedef spec_type_tag type;
};

// Specialization for the special default tag
template <>
struct elaborated_type_tag<spec___tag>
{
	typedef spec_class_tag type;
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

