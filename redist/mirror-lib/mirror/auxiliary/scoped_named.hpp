/**
 * @file mirror/auxiliary/scoped_named.hpp
 * @brief Internal implementation of scoped/named objects
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_SCOPED_NAMED_1011291729_HPP
#define MIRROR_AUX_SCOPED_NAMED_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/auxiliary/using_directive_fwd.hpp>
#include <mirror/auxiliary/named.hpp>
#include <string>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class BaseMetaObject>
struct scoped_named_impl;

struct scoped_named_base
{
protected:
	static std::string do_make_local_name(
		const char* base_name,
		size_t base_name_length,
		std::string local_name
	)
	{
		// NOTE: at this point the local name contains
		// the full name of the parent scope and we are
		// just reusing the variable later in this function
		//
		// We'll need the using directive list
		using_directives_list used;
		// If the parent scope is 'used' by the using directive
		if(used.registered(local_name))
		{
			// Return just the base name
			return std::string(base_name, base_name_length);
		}
		// Now, check if a prefix of the name has been registered
		size_t prefix_len = used.prefix_registered(local_name);
		// if it has chop it away including the two colons
		if(prefix_len != 0) local_name.erase(0, prefix_len+2);
		// append the separating double colon
		local_name.append("::");
		// and append the base name of this meta-object
		local_name.append(base_name, base_name_length);
		// return the result
		return local_name;
	}

	// All scoped objects have an ultimate ancestor
	// in the global scope
	static inline std::string make_full_name(
		_namespace::_*,
		_namespace::_*
	)
	{
		// return an empty string
		return std::string();
	}

	// All scoped objects have an ultimate ancestor
	// in the global scope
	static inline std::string make_local_name(
		_namespace::_*,
		_namespace::_*
	)
	{
		// return an empty string
		return std::string();
	}

	// Returns the full name of the parent scope
	template <typename PBMO>
	static inline std::string parent_full_name(PBMO*)
	{
		return std::string(scoped_named_impl<PBMO>::full_name());
	}

	// General implementation of make_full_name for
	// other base meta-objects than the global scope
	// placeholder nested in a named namespace
	template <typename PBMO, typename BMO>
	static std::string make_full_name(PBMO* pbmo, BMO*)
	{
		// First get the full name of the parent scope
		std::string result(parent_full_name(pbmo));
		// append the separating double colon
		result.append("::");
		// and append the base name of this meta-object
		result.append(BMO::name(), BMO::name_length());
		// return the result
		return result;
	}

	// General implementation of make_full_name for
	// other base meta-objects than the global scope
	// placeholder nested in a named namespace
	template <typename PBMO, typename BMO>
	static inline std::string make_local_name(PBMO* pbmo, BMO*)
	{
		return do_make_local_name(
			BMO::name(),
			BMO::name_length(),
			parent_full_name(pbmo)
		);
	}

	// General implementation of make_full_name for
	// other base meta-objects than the global scope
	// placeholder defined in the global scope
	template <typename BMO>
	static inline std::string make_full_name(_namespace::_*, BMO*)
	{
		// if it is something defined in the global scope
		// just use the base name of this meta-object
		return std::string(BMO::name(), BMO::name_length());
	}

	// General implementation of make_local_name for
	// other base meta-objects than the global scope
	// placeholder defined in the global scope
	template <typename BMO>
	static std::string make_local_name(_namespace::_*, BMO*)
	{
		// if it is something defined in the global scope
		// just use the base name of this meta-object
		return std::string(BMO::name(), BMO::name_length());
	}
};

// Implementation of scoped-named meta-objects
/* This helper template class implements part of the public
 * interface of meta-objects conforming to the MetaNamedScoped
 * concept.
 */
template <class BaseMetaObject>
struct scoped_named_impl : scoped_named_base
{
private:
	typedef scoped_named_base base;
public:
	// The base name getter function
	static std::string base_name(void)
	{
		return std::string(
			BaseMetaObject::name(),
			BaseMetaObject::name_length()
		);
	}

	// The full name getter function
	static std::string full_name(void)
	{
		// type of these pointers are used in the
		// dispatching between the overloads
		// of the make_full_name helper function
		typename BaseMetaObject::scope* ppbmo = nullptr;
		BaseMetaObject* pbmo = nullptr;
		return base::make_full_name(ppbmo, pbmo);
	}

	// The local name getter function
	static std::string local_name(void)
	{
		// type of these pointers are used in the
		// dispatching between the overloads
		// of the make_full_name helper function
		typename BaseMetaObject::scope* ppbmo = nullptr;
		BaseMetaObject* pbmo = nullptr;
		// return the local name
		return base::make_local_name(ppbmo, pbmo);
	}

	template <class Full, class Local>
	static inline std::string _hlp_build_name(
		Full /*full*/,
		Local /*local*/,
		std::string& /*left*/,
		std::string& /*right*/,
		std::string& /*extent*/,
		std::string& /*arg*/
	)
	{
		if(Local::value) return local_name();
		else if(Full::value) return full_name();
		else return base_name();
	}

};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

