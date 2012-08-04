/**
 * @file mirror/auxiliary/by_name.hpp
 * @brief Implementation of the by_name meta-function returning meta-variables
 * reflecting class member variables by its name
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUXILIARY_BY_NAME_1012132135_HPP
#define MIRROR_AUXILIARY_BY_NAME_1012132135_HPP

#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN

namespace aux {

template <class MetaVars>
struct by_name_hlpr;

template <class ... MetaVars>
struct by_name_hlpr<mp::range<MetaVars...> >
 : MetaVars::template _by_name_typ<MetaVars>::type ...
{ };

} // namespace aux

MIRROR_NAMESPACE_END

#endif //include guard

