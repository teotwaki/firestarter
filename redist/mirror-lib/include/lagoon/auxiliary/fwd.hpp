/**
 *  .file lagoon/auxiliary/fwd.hpp
 *  .brief Forward declaration of auxiliary classes and functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUXILIARY_FWD_1104091004_HPP
#define LAGOON_AUXILIARY_FWD_1104091004_HPP

#include <mirror/meta_object_tags.hpp>
#include <lagoon/config.hpp>
#include <lagoon/utils.hpp>

LAGOON_NAMESPACE_BEGIN
namespace aux {

template <typename Specifier, typename Interface>
shared<Interface> get_inheritance_type_spec(void);

template <typename Specifier, typename Interface>
shared<Interface> get_access_type_spec(void);

template <typename Specifier, typename Interface>
shared<Interface> get_elaborated_type_spec(void);

template <typename Specifier, typename Interface>
shared<Interface> get_storage_class_spec(void);

template <typename Specifier, typename Interface>
shared<Interface> get_constness_spec(void);

template <typename MetaMetaObject, typename Interface>
shared<Interface> get_meta_object(mirror::meta_meta_object_tag);

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

