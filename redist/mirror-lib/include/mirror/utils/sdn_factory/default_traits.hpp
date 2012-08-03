/**
 * .file mirror/utils/sdn_factory/default_traits.hpp
 * .brief Default traits for the structured data notation factory framework
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_FACTORY_DEFAULT_TRAITS_1011291729_HPP
#define MIRROR_UTILS_SDN_FACTORY_DEFAULT_TRAITS_1011291729_HPP

#include <mirror/utils/lex_match.hpp>
#include <string>
#include <cctype>
#include <set>

MIRROR_NAMESPACE_BEGIN

// The general traits for the script factory plugin
template <
	class SDNRange,
	class SDNElement,
	class SDNProperty
>
class sdn_fact_traits
{
public:
	typedef SDNRange range;
	typedef SDNElement element;
	typedef SDNProperty property;

	class shared_data
	{
	private:
		std::set<std::string> shared_strings;
	public:
		std::set<std::string>& strings(void)
		{
			return shared_strings;
		}

		std::set<std::string>::const_iterator share(
			const std::string& str
		)
		{
			return shared_strings.insert(str).first;
		}
	};

	typedef typename std::set<std::string>::const_iterator
		shared_string_proxy;

	static inline const std::string& get_shared_string(
		shared_data& data,
		shared_string_proxy proxy
	)
	{
		return *proxy;
	}

	static inline shared_string_proxy share_string(
		shared_data& data,
		const std::string& str
	)
	{
		return data.share(str);
	}

	static inline std::string translate(
		shared_data& data,
		std::string str
	)
	{
		return str;
	}
};


MIRROR_NAMESPACE_END

#endif //include guard

