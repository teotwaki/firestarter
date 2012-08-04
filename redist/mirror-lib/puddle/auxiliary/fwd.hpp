/**
 * .file puddle/auxiliary/fwd.hpp
 * .brief Forward declarations
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_FWD_1103151301_HPP
#define PUDDLE_AUXILIARY_FWD_1103151301_HPP

#include <puddle/config.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename IntegralConstant>
struct integer;

template <typename Range>
struct range;

template <typename SpecifierTag>
struct specifier;

template <typename ContainerKindTag>
struct container_kind;

template <typename MetaMetaObject>
struct meta_meta_object;

#define PUDDLE_HLPR_FWD_DECL_MO(OBJECT, I, X) \
template <typename MetaObject> \
struct meta_ ## OBJECT;

MIRROR_FOR_EACH_META_OBJECT(PUDDLE_HLPR_FWD_DECL_MO, 0)

#undef PUDDLE_HLPR_FWD_DECL_MO

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

