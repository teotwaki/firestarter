/**
 * .file mirror/utils/quick_reg/free_variable.hpp
 * .brief Boost.Preprocessor-based quick registering macros
 *
 *  Do not include directly, include mirror/utils/quick_reg.hpp
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_QUICK_REG_FREE_VARIABLE_1011291729_HPP
#define MIRROR_UTILS_QUICK_REG_FREE_VARIABLE_1011291729_HPP

MIRROR_NAMESPACE_BEGIN

// helper macro for MIRROR_QREG_GLOBAL_SCOPE_VARIABLES
#define MIRROR_HLP_QREG_G_S_VAR(R, X, VAR) \
MIRROR_REG_GLOBAL_SCOPE_VARIABLE(_, VAR)

// helper macro for MIRROR_QREG_VARIABLES
#define MIRROR_HLP_QREG_FREE_VAR(R, SCOPE, VAR) \
MIRROR_REG_FREE_VARIABLE(_, SCOPE, VAR)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of global scope enumerations
/**
 *  @param VARIABLES a sequence of variable names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_QREG_FREE_VARIABLES
 *  @ingroup registering_macros
 */
#define MIRROR_QREG_GLOBAL_SCOPE_VARIABLES(VARIABLES)
#else
#define MIRROR_QREG_GLOBAL_SCOPE_VARIABLES(VARIABLES) \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_G_S_VAR, _, VARIABLES)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of enumerations
/**
 *  @param NAMESPACE the full namespace name where the variables are defined.
 *  @param VARIABLES a sequence of variable names enclosed in parenthesises
 *  for example (x)(y)(z)
 *
 *  @see MIRROR_QREG_GLOBAL_SCOPE_VARIABLES
 *  @ingroup registering_macros
 */
#define MIRROR_QREG_FREE_VARIABLES(NAMESPACE, VARIABLES)
#else
#define MIRROR_QREG_FREE_VARIABLES(NAMESPACE, VARIABLES) \
BOOST_PP_SEQ_FOR_EACH(MIRROR_HLP_QREG_FREE_VAR, NAMESPACE, VARIABLES)
#endif

MIRROR_NAMESPACE_END

#endif //include guard

