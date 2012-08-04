/**
 * @file mirror/preprocessor.hpp
 * @brief Various preprocessor utilities
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PREPROCESSOR_1011291729_HPP
#define MIRROR_PREPROCESSOR_1011291729_HPP

#include <mirror/config.hpp>

//  Macro expanding to nothing
#define MIRROR_PP_EMPTY()

// Macro expanding to a comma
#define MIRROR_PP_COMMA() ,

//  Macro doing the concatenation of two preprocessor tokens
#define MIRROR_PP_CAT(A, B) MIRROR_PP_CAT_I(A, B)
#define MIRROR_PP_CAT_I(A, B) A ## B

// Variadic macro expanding into its arguments
#define MIRROR_PP_EXPAND_ARGS(...) __VA_ARGS__

#endif //include guard

