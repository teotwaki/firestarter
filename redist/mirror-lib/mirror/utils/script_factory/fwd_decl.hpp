/**
 * .file mirror/utils/script_factory/fwd_decl.hpp
 * .brief Forward declarations for the script-parsing factory generator plugin
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_FWD_DECL_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_FWD_DECL_1011291729_HPP


MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Product, class Traits, class NativeType>
class script_fact_source;

template <class Element, class Traits>
class script_fact_seq_source;

} // namespace aux

template <class Traits>
class script_fact_data;

template <class Product, class Traits>
class script_fact_manuf;

template <class Product, class Traits>
class script_fact_suppl;

template <class Product, class Traits>
class script_fact_enum;

MIRROR_NAMESPACE_END

#endif //include guard

