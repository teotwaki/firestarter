/**
 *  @file mirror/auxiliary/function_type_name.hpp
 *  @brief Helpers for composing function typenames
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_FUNCTION_TYPE_NAME_1011291729_HPP
#define MIRROR_AUX_FUNCTION_TYPE_NAME_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/identity.hpp>
#include <mirror/auxiliary/decorated_type_name.hpp>
#include <mirror/auxiliary/argument_type_list.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Function type name builder
template <
	typename RetValType,
	typename CallingConvention,
	typename ArgTypeRange
>
struct function_type_name_base : arg_type_list_builder
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
		using mirror::mp::identity;
		// the return value type
		typedef meta_type<RetValType> meta_RV;
		//
		std::string rv_left;
		std::string rv_right;
		std::string rv_extent;
		std::string rv_arg;
		std::string rv_t(meta_RV::_hlp_build_name(
			full,
			local,
			rv_left,
			rv_right,
			rv_extent,
			rv_arg
		));
		//
		// return value
		left.append(rv_left);
		left.append(rv_t);
		left.append(rv_right);
		left.append(rv_extent);
		// open pars.
		left.append(" ");
		left.append("(");
		// the calling convention
		left.append(CallingConvention::name());
		arg.append(")");
		//
		// argument list
		arg.append("(");
		append_args(ArgTypeRange(), arg, full, local);
		arg.append(")");
		// the rest of the ret val type
		arg.append(rv_arg);
		// return an empty string
		return std::string();
	}
};

template <
	typename RetValType,
	typename CallingConvention,
	typename ArgTypeRange
>
struct function_type_name
: decorated_type_name_finisher<
	function_type_name_base<
		RetValType,
		CallingConvention,
		ArgTypeRange
	>
> { };



} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

