/**
 * @file rubber/operations/write_to_ostream.hpp
 * @brief Write to std::ostream operation for meta-types
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_OPERATION_WRITE_TO_OSTREAM_1103151301_HPP
#define RUBBER_OPERATION_WRITE_TO_OSTREAM_1103151301_HPP

#include <rubber/config.hpp>

#include <iostream>

RUBBER_NAMESPACE_BEGIN
namespace aux {

class ostream_wr_base
{
public:
	template <typename T>
	static std::true_type _is_writable(
		T* x,
		decltype(std::cout << (*x) == std::cout) = false
	);
	static std::false_type _is_writable(...);

	template <typename T>
	static auto available(const T&) ->
	decltype(_is_writable((T*)nullptr));

	typedef std::false_type mutating;
protected:
	template <typename T>
	static void write(std::ostream& out, const T& value)
	{
		out << value;
	}
};

} // namespace aux

class write_to_cout : public aux::ostream_wr_base
{
public:
	template <typename T>
	static void apply(const T& value, mirror::some&)
	{
		write(std::cout, value);
	}
};

RUBBER_NAMESPACE_END

#endif //include guard

