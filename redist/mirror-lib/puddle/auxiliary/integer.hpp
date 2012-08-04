/**
 * .file puddle/auxiliary/integer.hpp
 * .brief The integral constant wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_INTEGER_1103151313_HPP
#define PUDDLE_AUXILIARY_INTEGER_1103151313_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/base_wrap.hpp>
#include <type_traits>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename T, T Value>
struct integer<std::integral_constant<T, Value> >
 : base_wrapper<std::integral_constant<T, Value> >
{
	static std::integral_constant<T, Value> static_value(void)
	{
		return std::integral_constant<T, Value>();
	}

	static T value(void)
	{
		return Value;
	}

	operator T (void) const
	{
		return Value;
	}

	integer<std::integral_constant<T, !Value> >
	operator ! (void) const
	{
		return integer<std::integral_constant<T, !Value> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value == OtherValue> >
	operator == (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value == OtherValue>>();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value != OtherValue> >
	operator != (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value != OtherValue>>();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value <  OtherValue> >
	operator <  (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value <  OtherValue>>();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, (Value > OtherValue)> >
	operator >  (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, (Value >OtherValue)>>();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value <= OtherValue> >
	operator <= (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value <= OtherValue>>();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value >= OtherValue> >
	operator >= (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value >= OtherValue>>();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value+OtherValue> >
	operator + (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value+OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value-OtherValue> >
	operator - (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value-OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value*OtherValue> >
	operator * (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value*OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value/OtherValue> >
	operator / (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value/OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value%OtherValue> >
	operator % (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value%OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value&OtherValue> >
	operator & (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value&OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value|OtherValue> >
	operator | (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value|OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value&&OtherValue> >
	operator && (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value&&OtherValue> >();
	}

	template <T OtherValue>
	friend integer<std::integral_constant<T, Value||OtherValue> >
	operator || (integer, integer<std::integral_constant<T, OtherValue> >)
	{
		return integer<std::integral_constant<T, Value||OtherValue> >();
	}
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

