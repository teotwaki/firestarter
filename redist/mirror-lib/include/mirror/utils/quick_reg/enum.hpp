/**
 * .file mirror/utils/quick_reg/pod_class.hpp
 * .brief Boost.Preprocessor-based quicke registering macros
 *
 *  Do not include directly, include mirror/utils/quick_reg.hpp
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_QUICK_REG_ENUM_1011291729_HPP
#define MIRROR_UTILS_QUICK_REG_ENUM_1011291729_HPP

MIRROR_NAMESPACE_BEGIN

// helper macro for MIRROR_QREG_ENUM
#define MIRROR_HLP_QREG_ENUM_VAL(R, X, ELEM) \
MIRROR_REG_ENUM_VALUE(ELEM)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of global scope enumerations
/**
 *  @param ENUM the base enum name.
 *  @param VALUES a sequence of enumeration value names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_QREG_ENUM
 *  @see MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup quick_registering_macros
 */
#define MIRROR_QREG_GLOBAL_SCOPE_ENUM(ENUM, VALUES)
#else
#define MIRROR_QREG_GLOBAL_SCOPE_ENUM(ENUM, VALUES) \
MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN(ENUM) \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_ENUM_VAL, _, VALUES) \
MIRROR_REG_ENUM_END
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of enumerations
/**
 *  @param NAMESPACE the full namespace name inside of which the enum is nested.
 *  @param ENUM the base enum name.
 *  @param VALUES a sequence of enumeration value names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_REG_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup quick_registering_macros
 */
#define MIRROR_QREG_ENUM(NAMESPACE, ENUM, VALUES)
#else
#define MIRROR_QREG_ENUM(NAMESPACE, ENUM, VALUES) \
MIRROR_REG_ENUM_BEGIN(NAMESPACE, ENUM) \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_ENUM_VAL, _, VALUES) \
MIRROR_REG_ENUM_END
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of enumerations nested in a class
/**
 *  @param PARENT_CLASS the full class name inside of which the enum is nested.
 *  @param ENUM the base enum name.
 *  @param VALUES a sequence of member variable names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_REG_NESTED_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup quick_registering_macros
 */
#define MIRROR_QREG_NESTED_ENUM(PARENT_CLASS, ENUM, VALUES)
#else
#define MIRROR_QREG_NESTED_ENUM(PARENT_CLASS, ENUM, VALUES) \
MIRROR_REG_NESTED_ENUM_BEGIN(PARENT_CLASS, ENUM) \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_ENUM_VAL, _, VALUES) \
MIRROR_REG_ENUM_END
#endif

MIRROR_NAMESPACE_END

#endif //include guard

