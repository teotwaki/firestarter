/**
 *  @file mirror/auxiliary/using_directive.hpp
 *  @brief Implementation of the Mirror's using directive utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_USING_DIRECTIVE_1011291729_HPP
#define MIRROR_AUX_USING_DIRECTIVE_1011291729_HPP

#include <mirror/auxiliary/using_directive_fwd.hpp>

#include <string>
#include <map>


MIRROR_NAMESPACE_BEGIN
namespace aux {

::std::map<unsigned, std::string>&
using_directives_list::reg_ref(void)
{
	static ::std::map<unsigned, std::string> reg;
	return reg;
}

template <typename MetaObject>
void using_directives_list::reg_object(unsigned uid)
{
	reg_ref()[uid] = MetaObject::full_name();
}

template <typename MetaObject>
void using_directives_list::unreg_object(unsigned uid)
{
	reg_ref().erase(uid);
}

bool using_directives_list::registered(const std::string& full_name)
{
	auto i = reg_ref().begin();
	auto e = reg_ref().end();
	while(i != e)
	{
		if(i->second == full_name)
			return true;
		++i;
	}
	return false;
}

typename std::string::size_type
using_directives_list::prefix_registered(const std::string& full_name)
{
	auto i = reg_ref().begin();
	auto e = reg_ref().end();
	typename std::string::size_type longest = 0;
	typename std::string::size_type current = 0;
	while(i != e)
	{
		// if the i-th registered name is a prefix
		// to the full_name passed as parameter
		if(full_name.find(i->second) == 0)
		{
			// get its length
			current = i->second.size();
			// and if the current is longer than longest
			// remember it
			if(longest < current)
				longest = current;
		}
		++i;
	}
	// return the longest prefix length
	return longest;
}

// This template is used by the MIRROR_USING(META_OBJECT)
template <typename MetaObject>
class _using
{
private:
	using_directives_list used;
	const unsigned uid;
public:
	_using(unsigned _uid)
	 : uid(_uid)
	{
		used.reg_object<MetaObject>(uid);
	}

	~_using(void)
	{
		used.unreg_object<MetaObject>(uid);
	}
};


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

