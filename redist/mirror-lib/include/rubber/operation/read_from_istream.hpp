/**
 * @file rubber/operations/read_from_istream.hpp
 * @brief Read from std::istream operation for meta-types
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_OPERATION_READ_FROM_ISTREAM_1103151301_HPP
#define RUBBER_OPERATION_READ_FROM_ISTREAM_1103151301_HPP

#include <rubber/config.hpp>

#include <iostream>

RUBBER_NAMESPACE_BEGIN
namespace aux {

class istream_rd_base
{
public:
	template <typename T>
	static std::true_type _is_readable(
		T* x,
		decltype(std::cin >> (*x) == std::cin) = false
	);
	static std::false_type _is_readable(...);

	template <typename T>
	static auto available(T&) ->
	decltype(_is_readable((T*)nullptr));

	typedef std::true_type mutating;
protected:
	template <typename T>
	static void read(std::istream& in, T& value)
	{
		in >> value;
	}
};

} // namespace aux

class read_from_cin : public aux::istream_rd_base
{
public:
	template <typename T>
	static void apply(T& value, mirror::some&)
	{
		read(std::cin, value);
	}
};

RUBBER_NAMESPACE_END

#endif //include guard

