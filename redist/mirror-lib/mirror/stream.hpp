/**
 * @file mirror/stream.hpp
 * @brief Mirror std iostream utilities
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_STREAM_1011291729_HPP
#define MIRROR_STREAM_1011291729_HPP

#include <mirror/stream/json.hpp>
#include <mirror/stream/xml.hpp>
#include <mirror/stream/meta_xml.hpp>
#include <mirror/stream/simple_struct.hpp>

MIRROR_NAMESPACE_BEGIN
/// Namespace containing simple reflection-based stream writers
/** In this namespace are implemented several wrappers which
 *  allow to write instances of arbitrary classes that are registered
 *  with Mirror into a standard output stream in various formats.
 */
namespace stream { }

MIRROR_NAMESPACE_END

#endif //include guard

