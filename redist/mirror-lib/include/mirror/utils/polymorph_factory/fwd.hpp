/**
 * .file mirror/utils/polymorph_factory/fwd.hpp
 * .brief forward declaration of some parts of the polymorphic factory
 *  template
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_FWD_1011291729_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_FWD_1011291729_HPP

MIRROR_NAMESPACE_BEGIN

// Forward declaration of manufacturer
template <class Product, class SourceTraits>
class polymorph_factory_manufacturer_holder;

// Forward declaration of enumerator
template <class Product, class SourceTraits>
class polymorph_factory_enumerator_holder;

// Forward declaration of suppliers
template <class Product, class SourceTraits>
class polymorph_factory_suppliers_holder;

MIRROR_NAMESPACE_END

#endif // include guard

