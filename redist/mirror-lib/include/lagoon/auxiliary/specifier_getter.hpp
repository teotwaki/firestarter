/**
 *  .file lagoon/auxiliary/specifier_getter.hpp
 *  .brief Implementation of an internal helper function for
 *   creating instances of specifier implementations
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_SPECIFIER_GETTER_1011291729_HPP
#define LAGOON_AUX_SPECIFIER_GETTER_1011291729_HPP

#include <lagoon/specifiers.hpp>
#include <lagoon/utils.hpp>
#include <mirror/specifier_tags.hpp>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Function creating inheritance type specifiers
template <typename Specifier, typename Interface>
inline shared<Interface> get_inheritance_type_spec(void)
{
	Specifier _spec;
	static inheritance_type_specifier spec(_spec);
	return shared<Interface>(&spec);
}

// Function creating access type specifiers
template <typename Specifier, typename Interface>
inline shared<Interface> get_access_type_spec(void)
{
	Specifier _spec;
	static access_type_specifier spec(_spec);
	return shared<Interface>(&spec);
}

// Function creating elaborated type specifiers
template <typename Specifier, typename Interface>
inline shared<Interface> get_elaborated_type_spec(void)
{
	Specifier _spec;
	static elaborated_type_specifier spec(_spec);
	return shared<Interface>(&spec);
}

// Function creating storage class specifiers
template <typename Specifier, typename Interface>
inline shared<Interface> get_storage_class_spec(void)
{
	Specifier _spec;
	static storage_class_specifier spec(_spec);
	return shared<Interface>(&spec);
}

// Function creating storage class specifiers
template <typename Specifier, typename Interface>
inline shared<Interface> get_constness_spec(void)
{
	Specifier _spec;
	static constness_specifier spec(_spec);
	return shared<Interface>(&spec);
}


} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

