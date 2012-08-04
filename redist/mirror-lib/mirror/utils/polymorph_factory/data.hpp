/**
 * .file mirror/utils/polymorph_factory/data.hpp
 * .brief data for the polymorphic factory template
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_POLYMORPH_FACTORY_DATA_1011291729_HPP
#define MIRROR_UTILS_POLYMORPH_FACTORY_DATA_1011291729_HPP

#include <cassert>

MIRROR_NAMESPACE_BEGIN

// Data used during the factory generation process
template <class SourceTraits>
struct polymorph_factory_data
{
	typedef typename SourceTraits::builder builder;
	typedef typename SourceTraits::data polymorph_data;

	builder* factory_builder;
	polymorph_data factory_data;

	polymorph_factory_data(builder* bldr)
	 : factory_builder(bldr)
	{
		assert(factory_builder!= nullptr);
	}

	polymorph_factory_data(
		const polymorph_factory_data& other,
		const polymorph_data& data
	): factory_builder(other.factory_builder)
	 , factory_data(data)
	{
		assert(factory_builder!= nullptr);
	}
};

MIRROR_NAMESPACE_END

#endif // include guard

