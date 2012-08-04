/**
 *  @file mirror/auxiliary/argument_type_list.hpp
 *  @brief Helpers for composing function and template arg lists
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_ARGUMENT_TYPE_LIST_1011291729_HPP
#define MIRROR_AUX_ARGUMENT_TYPE_LIST_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/for_each.hpp>
#include <mirror/auxiliary/decorated_type_name.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

/*  Helper class implementing a function that can be used
 *  to append the list of type names to the given string.
 */
struct arg_type_list_builder
{
private:
	template <
		typename String,
		typename Full,
		typename Local
	>
	struct arg_appender
	{
		// the destination string
		String& dest;

		arg_appender(String& str)
		 : dest(str)
		{ }

		template <typename IterInfo>
		void operator()(IterInfo) const
		{
			// get and reflect the argument type
			typedef typename IterInfo::type arg_type;
			typedef meta_type<arg_type> meta_arg_type;
			// append the type name
			if(Local::value)
				dest.append(meta_arg_type::local_name());
			else if(Full::value)
				dest.append(meta_arg_type::full_name());
			else dest.append(meta_arg_type::base_name());
			// append the comma
			if(!IterInfo::is_last::value)
				dest.append(", ");
		}
	};
protected:
	template <
		typename ArgTypeRange,
		typename String,
		typename Full,
		typename Local
	>
	static void append_args(
		ArgTypeRange,
		String& str,
		Full full_or_base,
		Local local
	)
	{
		// helper functor for appending the argument list
		// to the destination string
		arg_appender<String, Full, Local> func(str);
		// use the functor with the for-each algorithm
		mp::for_each_ii<ArgTypeRange>(func);
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

