/**
 * .file mirror/utils/sdn_factory/error.hpp
 * .brief Forward declarations for the structured data factory framework
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_FACTORY_ERROR_1011291729_HPP
#define MIRROR_UTILS_SDN_FACTORY_ERROR_1011291729_HPP

#include <stdexcept>

MIRROR_NAMESPACE_BEGIN

class sdn_fact_node_not_found_error : public std::runtime_error
{
public:
	sdn_fact_node_not_found_error(const std::string& _what)
	 : std::runtime_error(_what)
	{ }
};

MIRROR_NAMESPACE_END

#endif //include guard

