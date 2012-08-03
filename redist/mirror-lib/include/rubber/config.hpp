/**
 * @file rubber/config.hpp
 * @brief Configuration options
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_CONFIG_1103151301_HPP
#define RUBBER_CONFIG_1103151301_HPP

#include <mirror/raw_ptr/raw_ptr.hpp>
#include <mirror/utils/some_type.hpp>

// Macro that begins the Rubber library namespace
#define RUBBER_NAMESPACE_BEGIN \
namespace rubber {

// Macro which ends the Rubber library namespace
#define RUBBER_NAMESPACE_END \
} /* namespace rubber */

/** @defgroup rubber_config_options Rubber - Configuration options
 *
 *  The polymorphic nature of the meta-objects can make Rubber-using
 *  applications prone to have long compilation times
 *  and large resulting executables.
 *  Rubber is however configurable via preprocessor options,
 *  and lets the user specify which features should
 *  be included in the meta-object and what should be ommited. This
 *  way the application does not pay for things it does not use
 *  in terms of resources necessary for the compilation and the resulting
 *  code size.
 */

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def RUBBER_POLYMORPHIC_META_OBJECT
 *  @brief Enables downcasting of Rubber's meta-objects to their specializations
 *
 *  Setting this symbol to a nonzero integer value enables downcasting
 *  of references to meta-objects to their specializations. Setting it to zero
 *  (the default) disables this feature.
 *
 *  It is important to note, that when this feature is enabled, the meta-objects
 *  must be handled by const reference. Also enabling polymorphic meta-objects
 *  can dramatically increase the compilation times and the resulting executable
 *  code size so unless this feature is really necessary it is suggested to keep
 *  it disabled. Also the Lagoon run-time layer may be a better option in such
 *  cases.
 *
 *  @ingroup rubber_config_options
 */
#define RUBBER_POLYMORPHIC_META_OBJECT 0
#endif
#ifndef RUBBER_POLYMORPHIC_META_OBJECT
#define RUBBER_POLYMORPHIC_META_OBJECT 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def RUBBER_NO_NAMESPACE_MEMBERS
 *  @brief Disables the traversal of namespace members
 *
 *  If this is defined as zero then namespace members can be iterated,
 *  otherwise the members() function for meta-namespace
 *  returns an empty range.
 *
 *  Setting this to a non-zero value can greatly improve compilation
 *  times if namespace member traversal is not needed.
 *
 *  @ingroup rubber_config_options
 */
#define RUBBER_NO_NAMESPACE_MEMBERS 0
#endif
#ifndef RUBBER_NO_NAMESPACE_MEMBERS
#define RUBBER_NO_NAMESPACE_MEMBERS 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def RUBBER_MNSO_WITH_LOCAL_NAME
 *  @brief Enables the meta_named_scoped_objects's local_name member function
 *
 *  Setting this to a nonzero integer value enables the
 *  @link rubber::meta_named_scoped_object::local_name local_name@endlink
 *  member function in meta_named_scoped_object.
 *
 *  @ingroup rubber_config_options
 */
#define RUBBER_MNSO_WITH_LOCAL_NAME 0
#endif
#ifndef RUBBER_MNSO_WITH_LOCAL_NAME
#define RUBBER_MNSO_WITH_LOCAL_NAME 0
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def RUBBER_META_TYPE_OPERATIONS
 *  @brief Defines a set of operations which will be available for reflected types
 *
 *  If defined, this PP symbol must contain a comma-separated list of types
 *  that conform to the rubber::TypeOperation concept.
 *  All operations listed can than be executed on type-erased instances via
 *  their meta-types'
 *  @link rubber::meta_type::examine_by examine_by @endlink or
 *  @link rubber::meta_type::modify_by modify_by @endlink member functions.
 *
 *  If this PP symbol is not defined then the member functions listed above
 *  are not available in the meta_type meta-object.
 *
 *  @ingroup rubber_config_options
 */
#define RUBBER_META_TYPE_OPERATIONS comma,separated,list,of,operation,wrappers
#endif

RUBBER_NAMESPACE_BEGIN

using mirror::raw_ptr;
using mirror::raw_cast;

using mirror::some;
using mirror::some_ref;
using mirror::some_cast;

namespace aux {

inline int& meta_object_uid_seq(void)
{
	static int seq = 0;
	return seq;
}

template <typename MirrorMetaObject>
inline int meta_object_uid(MirrorMetaObject)
{
	static int uid = ++meta_object_uid_seq();
	return uid;
}

template <typename TypeErased>
struct return_value
{
#if RUBBER_POLYMORPHIC_META_OBJECT
	typedef const TypeErased& type;
#else
	typedef TypeErased type;
#endif
};

} // namespace aux

RUBBER_NAMESPACE_END

#endif //include guard

