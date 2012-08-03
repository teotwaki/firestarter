/**
 *  @file mirror/auxiliary/decorated_type_name.hpp
 *  @brief Helpers for composing nontrivial typenames
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_DECORATED_TYPE_NAME_1011291729_HPP
#define MIRROR_AUX_DECORATED_TYPE_NAME_1011291729_HPP

#include <mirror/auxiliary/static_int_to_str.hpp>
#include <mirror/ct_string/basic_string.hpp>
#include <string>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Helper class for building non trivial type names
template <class NamedScoped, class Decorator>
struct decorated_type_name_base
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
		// define the decorator type
		typedef typename Decorator::type decorator_type;
		// and construct the decorator
		// the decorators dectructor might
		// also kick in when the program flow
		// goes out of scope and this instance is
		// destroyed
		decorator_type decor(left, right, extent, arg);
		// use the named/scope type to build the base type
		return NamedScoped::_hlp_build_name(
			full,
			local,
			left,
			right,
			extent,arg
		);
	}
};

// This class finished the decoration process for a particular type
template <class Base>
struct decorated_type_name_finisher : public Base
{
protected:
	template <class Bool1, class Bool2>
	static std::string init_name(Bool1 full, Bool2 local)
	{
		std::string left, right, extent, arg;
		std::string temp(_hlp_build_name(
			full,
			local,
			left,
			right,
			extent,
			arg
		));
		left.append(temp);
		left.append(right);
		left.append(extent);
		left.append(arg);
		return left;
	}
public:
	typedef typename mirror::cts::string<>::type static_name;

	static std::string base_name(void)
	{
		return init_name(
			std::false_type(),
			std::false_type()
		);
	}

	static std::string full_name(void)
	{
		return init_name(
			std::true_type(),
			std::false_type()
		);
	}

	static std::string local_name(void)
	{
		return init_name(std::true_type(), std::true_type());
	}
};

/*  This template should be used to decorate typenames
 *  using custom decorators.
 */
template <class NamedScoped, class Decorator>
struct decorated_type_name
 : decorated_type_name_finisher<
	decorated_type_name_base<NamedScoped, Decorator>
> { };

template <typename T> struct type_name_decor;

/*  class for decorators that append something to 'right'
 *  part of the type name
 */
template <class Decoration>
struct type_name_right_postfix_decor
{
	inline type_name_right_postfix_decor(
		std::string&,
		std::string& _right,
		std::string&,
		std::string&
	) : right(_right) { }

	inline ~type_name_right_postfix_decor(void)
	{
		right.append(Decoration::c_str());
	}
	std::string& right;

	typedef type_name_right_postfix_decor type;
};

struct do_nothing_decor
{
	struct type
	{
		inline type(
			std::string&,
			std::string&,
			std::string&,
			std::string&
		){ }
	};
};


// plain type decorator
template <typename T>
struct type_name_decor
 : do_nothing_decor
{ };

struct ct_string_ast
{
	static const char* c_str(void)
	{
		return " *";
	}
};


// pointer decorator
template <typename T>
struct type_name_decor<T*>
: type_name_right_postfix_decor<
	ct_string_ast
> { };

struct ct_string_amp
{
	static const char* c_str(void)
	{
		return " &";
	}
};


// reference decorator
template <typename T>
struct type_name_decor<T&>
: type_name_right_postfix_decor<
	ct_string_amp
> { };

struct ct_string_dbl_amp
{
	static const char* c_str(void)
	{
		return " &&";
	}
};

// r-value reference decorator
template <typename T>
struct type_name_decor<T&&>
: type_name_right_postfix_decor<
	ct_string_dbl_amp
> { };

struct ct_string_const
{
	static const char* c_str(void)
	{
		return " const";
	}
};

// const type decorator
template <typename T>
struct type_name_decor<const T>
: type_name_right_postfix_decor<
	ct_string_const
> { };

struct ct_string_volatile
{
	static const char* c_str(void)
	{
		return " volatile";
	}
};

// volatile type decorator
template <typename T>
struct type_name_decor<volatile T>
: type_name_right_postfix_decor<
	ct_string_volatile
> { };

struct ct_string_cv
{
	static const char* c_str(void)
	{
		return " const volatile";
	}
};

// const volatile type decorator
template <typename T>
struct type_name_decor<const volatile T>
: type_name_right_postfix_decor<
	ct_string_cv
> { };

// array decorator
template <typename T>
struct type_name_decor< T[] >
{
	struct type
	{
		inline type(
			std::string&,
			std::string&,
			std::string& _extent,
			std::string&
		)
		{
			if(_extent.empty()) _extent.append(" ");
			_extent.append("[]");
		}
	};
};

// array decorator
template <typename T, size_t Size>
struct type_name_decor< T[ Size ] >
{
	struct type
	{
	private:
		static std::string init_postfix(void)
		{
			typedef typename aux::int_to_str<Size> size_string;
			std::string res("[");
			//
			// setup a buffer for the number
			const size_t max_size = size_string::length::value+1;
			char buffer[max_size];
			// put it into the buffer
			size_string::convert(buffer, max_size);
			// append the buffer
			res.append(std::string(buffer));
			// append ']'
			res.append("]");
			return res;
		}
	public:
		inline type(
			std::string&,
			std::string&,
			std::string& _extent,
			std::string&
		)
		{
			static std::string s_postfix(init_postfix());
			if(_extent.empty()) _extent.append(" ");
			_extent.append(s_postfix);
		}
	};
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

