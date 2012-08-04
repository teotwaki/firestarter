/**
 * .file mirror/utils/quick_reg/pod_class.hpp
 * .brief Boost.Preprocessor-based quick registering macros
 *
 *  Do not include directly, include mirror/utils/quick_reg.hpp
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_QUICK_REG_POD_CLASS_1011291729_HPP
#define MIRROR_UTILS_QUICK_REG_POD_CLASS_1011291729_HPP

MIRROR_NAMESPACE_BEGIN

// helper macro for MIRROR_QREG_POD_CLASS
#define MIRROR_HLP_QREG_PLAIN_MEM_VAR(R, X, ELEM) \
MIRROR_REG_CLASS_MEM_VAR(_, _, _, ELEM)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of POD classes defined at the global scope
/**
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *  @param CLASS the base class name.
 *  @param MEMBERS a sequence of member variable names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_QREG_POD_CLASS
 *  @see MIRROR_REG_GLOBAL_SCOPE_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @ingroup registering_macros
 */
#define MIRROR_QREG_GLOBAL_SCOPE_POD_CLASS(ELABORATED_TYPE, CLASS, MEMBERS)
#else
#define MIRROR_QREG_GLOBAL_SCOPE_POD_CLASS(ELABORATED_TYPE, CLASS, MEMBERS) \
MIRROR_REG_GLOBAL_SCOPE_CLASS_BEGIN(ELABORATED_TYPE, CLASS) \
MIRROR_REG_CLASS_MEM_VARS_BEGIN \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_PLAIN_MEM_VAR, _, MEMBERS) \
MIRROR_REG_CLASS_MEM_VARS_END \
MIRROR_REG_CONSTRUCTORS_BEGIN \
MIRROR_REG_DEFAULT_CONSTRUCTOR(public) \
MIRROR_REG_COPY_CONSTRUCTOR(public) \
MIRROR_REG_STRUCT_INITIALIZER() \
MIRROR_REG_CONSTRUCTORS_END \
MIRROR_REG_CLASS_END
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of POD class without base classes, etc.
/**
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *  @param NAMESPACE the full namespace name inside of which the class is
 *  nested.
 *  @param CLASS the base class name.
 *  @param MEMBERS a sequence of member variable names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_QREG_POD_CLASS(ELABORATED_TYPE, NAMESPACE, CLASS, MEMBERS)
#else
#define MIRROR_QREG_POD_CLASS(ELABORATED_TYPE, NAMESPACE, CLASS, MEMBERS) \
MIRROR_REG_CLASS_BEGIN(ELABORATED_TYPE, NAMESPACE, CLASS) \
MIRROR_REG_CLASS_MEM_VARS_BEGIN \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_PLAIN_MEM_VAR, _, MEMBERS) \
MIRROR_REG_CLASS_MEM_VARS_END \
MIRROR_REG_CONSTRUCTORS_BEGIN \
MIRROR_REG_DEFAULT_CONSTRUCTOR(public) \
MIRROR_REG_COPY_CONSTRUCTOR(public) \
MIRROR_REG_STRUCT_INITIALIZER() \
MIRROR_REG_CONSTRUCTORS_END \
MIRROR_REG_CLASS_END
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of POD classes nested in other classes
/**
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *  @param PARENT_CLASS the full class name inside of which the registerd class
 *  is nested.
 *  @param CLASS the base class name.
 *  @param MEMBERS a sequence of member variable names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_QREG_POD_CLASS
 *  @see MIRROR_REG_NESTED_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @ingroup registering_macros
 */
#define MIRROR_QREG_NESTED_POD_CLASS(ELABORATED_TYPE, PARENT_CLASS, CLASS, MEMBERS)
#else
#define MIRROR_QREG_NESTED_POD_CLASS(ELABORATED_TYPE, PARENT_CLASS, CLASS, MEMBERS)\
MIRROR_REG_NESTED_CLASS_BEGIN(ELABORATED_TYPE, PARENT_CLASS, CLASS) \
MIRROR_REG_CLASS_MEM_VARS_BEGIN \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_PLAIN_MEM_VAR, _, MEMBERS) \
MIRROR_REG_CLASS_MEM_VARS_END \
MIRROR_REG_CONSTRUCTORS_BEGIN \
MIRROR_REG_DEFAULT_CONSTRUCTOR(public) \
MIRROR_REG_COPY_CONSTRUCTOR(public) \
MIRROR_REG_STRUCT_INITIALIZER() \
MIRROR_REG_CONSTRUCTORS_END \
MIRROR_REG_CLASS_END
#endif

MIRROR_NAMESPACE_END

#endif //include guard

