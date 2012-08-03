/**
 *  @file mirror/auxiliary/using_directive_fwd.hpp
 *  @brief Forward declaration of the Mirror's using directive utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_USING_DIRECTIVE_FWD_1011291729_HPP
#define MIRROR_AUX_USING_DIRECTIVE_FWD_1011291729_HPP

#include <string>
#include <map>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Monostate class for registering constructs to be used without scope name
/* This class is used internally to store the Mirror's using directives
 * on the global and the local scopes.
 */
class using_directives_list
{
private:
	// returns a reference to the registry storing full names
	// of constructs used with the using directive
	static inline std::map<unsigned, std::string>& reg_ref(void);
public:
	// regiters a meta object with the given uid
	template <typename MetaObject>
	void reg_object(unsigned uid);

	// unregiters a meta object with the given uid
	template <typename MetaObject>
	void unreg_object(unsigned uid);

	// checks if a meta-object with the given name is registered
	bool registered(const std::string& full_name);

	// checks is a prefix for the given name is registered
	// and returns the lenght of the prefix
	typename std::string::size_type
	prefix_registered(const std::string& full_name);

};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

