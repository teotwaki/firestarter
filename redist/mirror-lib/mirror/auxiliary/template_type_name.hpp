/**
 *  @file mirror/auxiliary/template_type_name.hpp
 *  @brief Helpers for composing template type names
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_TEMPLATE_TYPE_NAME_1011291729_HPP
#define MIRROR_AUX_TEMPLATE_TYPE_NAME_1011291729_HPP

#include <mirror/auxiliary/decorated_type_name.hpp>
#include <mirror/auxiliary/argument_type_list.hpp>
#include <mirror/meta_prog/nil_term_pack.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Function type name builder
template <
	typename BaseNamedScoped,
	typename ArgTypeRange
>
struct template_type_name_base : arg_type_list_builder
{
public:
	template <class Bool1, class Bool2>
	static std::string _hlp_build_name(
		Bool1 full,
		Bool2 local,
		std::string& left,
		std::string& right,
		std::string& extent,
		std::string& arg
	)
	{
		// Get the name of the template
		std::string name(
			BaseNamedScoped::_hlp_build_name(
				full,
				local,
				left,
				right,
				extent,
				arg
			)
		);
		typedef arg_type_list_builder args;
		//
		right.append("< ");
		args::append_args(
			typename mirror::mp::remove_nil_types<
				ArgTypeRange
			>::type(),
			right,
			full,
			local
		);
		right.append(" >");
		//
		return name;
	}
};

template < class BaseNamedScoped, typename Type >
struct template_type_name;

template <
	class BaseNamedScoped,
	template <typename ...> class Template,
	typename ... Params
>
struct template_type_name<BaseNamedScoped, Template<Params...> >
: decorated_type_name_finisher<
	template_type_name_base<
		BaseNamedScoped,
		mp::range<Params...>
	>
> { };




} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

