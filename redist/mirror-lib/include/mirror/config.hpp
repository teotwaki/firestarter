/**
 * @file mirror/config.hpp
 * @brief Configuration options
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONFIG_1011291729_HPP
#define MIRROR_CONFIG_1011291729_HPP


// Macro that begins the Mirror library namespace
#define MIRROR_NAMESPACE_BEGIN \
namespace mirror {

// Macro which ends the Mirror library namespace
#define MIRROR_NAMESPACE_END \
} /* namespace mirror */ \

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which begins the registering with Mirror
/**
 *  @see MIRROR_REG_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_BEGIN
#else
#define MIRROR_REG_BEGIN MIRROR_NAMESPACE_BEGIN
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which ends the registering with Mirror
/**
 *  @see MIRROR_REG_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_END
#else
#define MIRROR_REG_END MIRROR_NAMESPACE_END
#endif


/** @defgroup mirror_config_options Mirror - Configuration options
 *
 *  Mirror allows for some compile-time configuration by the means
 *  of the C++ preprocessor. Setting various configuration options
 *  to different values influences the internal implementation details
 *  and the behavior of the library.
 */

#ifdef MIRROR_DOCUMENTATION_ONLY
/** @def MIRROR_NO_RTTI
 *  @brief Whether to disable the use of Run-time type information (RTTI)
 *
 *  Setting this symbol to a nonzero integer value disables the usage
 *  of RTTI by the Mirror library. This affects mainly the raw pointers.
 *
 *  Use of RTTI is enabled by default.
 *
 *  @ingroup mirror_config_options
 */
#define MIRROR_NO_RTTI 0 // or 1
#endif
#ifndef MIRROR_NO_RTTI
     // TODO: decide how to set this
#    define MIRROR_NO_RTTI 0
#endif

// TODO: remove this when the nullptr constant is supported
#define nullptr 0

#endif //include guard

