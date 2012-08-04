/**
 * .file puddle/auxiliary/container_kind.hpp
 * .brief The container_kind wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_CONTAINER_KIND_1103160725_HPP
#define PUDDLE_AUXILIARY_CONTAINER_KIND_1103160725_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/tag_wrap.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename ContainerKind>
struct container_kind_tag
: tag_wrapper<ContainerKind, mirror::non_container_tag>
{
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

