/**
 * .file mirror/utils/quick_reg/class.hpp
 * .brief Boost.Preprocessor-based quick registering macros
 *
 *  Do not include directly, include mirror/utils/quick_reg.hpp
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_QUICK_REG_CLASS_1011291729_HPP
#define MIRROR_UTILS_QUICK_REG_CLASS_1011291729_HPP


MIRROR_NAMESPACE_BEGIN

// Helper macro for MIRROR_QREG_CLASS
#define MIRROR_HLP_QREG_SIMPLE_BASE_CLASS(R, I, X, ELEM) \
BOOST_PP_COMMA() \
MIRROR_REG_BASE_CLASS(_, public, ELEM)

#define MIRROR_QREG_CLASS(ELABORATED_TYPE, SCOPE, NAME, ITEMS) \
MIRROR_REG_CLASS_BEGIN(ELABORATED_TYPE, SCOPE, NAME) \
MIRROR_REG_BASE_CLASSES_BEGIN \
MIRROR_REG_BASE_CLASS( \
	_, public, \
	BOOST_PP_SEQ_HEAD(BOOST_PP_TUPLE_ELEM(2, 0, ITEMS)) \
) \
BOOST_PP_SEQ_FOR_EACH_I( \
	MIRROR_HLP_QREG_SIMPLE_BASE_CLASS, \
	_, BOOST_PP_SEQ_TAIL(BOOST_PP_TUPLE_ELEM(2, 0, ITEMS)) \
) \
MIRROR_REG_BASE_CLASSES_END \
MIRROR_REG_CLASS_MEM_VARS_BEGIN \
BOOST_PP_SEQ_FOR_EACH( \
	MIRROR_HLP_QREG_PLAIN_MEM_VAR, \
	_, BOOST_PP_TUPLE_ELEM(2, 1, ITEMS) \
) \
MIRROR_REG_CLASS_MEM_VARS_END \
MIRROR_REG_CLASS_END


MIRROR_NAMESPACE_END

#endif //include guard

