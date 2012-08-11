/**
 * @file mirror/utils/enum_val_by_name.hpp
 * @brief Value by name getter for enumerated types
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_ENUM_VAL_BY_NAME_1101180748_HPP
#define MIRROR_UTILS_ENUM_VAL_BY_NAME_1101180748_HPP

#include <mirror/meta_enum.hpp>
#include <map>
#include <cstring>

MIRROR_NAMESPACE_BEGIN

/// A monostate utility class that allows to get enumerated value by its name
template <typename Enum>
class enum_value_by_name
{
private:
	struct ltcstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return std::strcmp(s1, s2) < 0;
		}
	};

	template <typename ... Idx>
	static std::map<const char*, Enum, ltcstr>
	cache_vals_by_names(mp::range<Idx ...>)
	{
		const std::pair<const char*, Enum> _init[] = {
			std::pair<const char*, Enum>(
				mirror::_enum::_<Enum>::item_name(Idx()),
				mirror::_enum::_<Enum>::item_val_c(Idx())
			)...
		};
		return std::map<const char*, Enum, ltcstr>(
			_init,
			_init + sizeof ... (Idx)
		);
	}

	static const std::map<const char*, Enum, ltcstr>&
	value_by_name_cache(void)
	{
		typename mp::make_index_seq<
			typename mirror::_enum::_<Enum>::item_count
		>::type indices;
		static auto cache = cache_vals_by_names(indices);
		return cache;
	}
public:
	/// Checks if the enumeration has a value with the given name
	static bool has(const std::string& name)
	{
		auto pos = value_by_name_cache().find(name.c_str());
		return (pos != value_by_name_cache().end());
	}

	/// Gets the enumerated value by name
	/** The name must be a valid name of one of the values,
	 *  otherwise this function will assert. To check if
	 *  the name exists use the has() member function
	 */
	static Enum get(const std::string& name)
	{
		auto pos = value_by_name_cache().find(name.c_str());
		assert(pos != value_by_name_cache().end());
		return pos->second;
	}

	/// Gets a pair of bool and Enum value by name
	/** The boolean value indicates whether a value with
	 *  the name passed as argument was found. If true
	 *  then the second member contains the found value
	 *  otherwise the value of the second member is
	 *  unspecified.
	 *
	 *  @see has
	 *  @see get
	 */
	static std::pair<bool, Enum> get_opt(const std::string& name)
	{
		auto pos = value_by_name_cache().find(name.c_str());
		if(pos == value_by_name_cache().end())
			return std::pair<bool, Enum>(false, Enum());
		return std::pair<bool, Enum>(true, pos->second);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

