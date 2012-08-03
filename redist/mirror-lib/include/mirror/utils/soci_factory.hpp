/**
 * @file mirror/utils/soci_factory.hpp
 * @brief postgresql/soci-based factory template for the factory generator
 * utility
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SOCI_FACTORY_1011291729_HPP
#define MIRROR_UTILS_SOCI_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sql_factory.hpp>
#include <soci.h>

MIRROR_NAMESPACE_BEGIN

class soci_fact_data
{
private:
	typedef soci::rowset<soci::row> rowset;
	rowset::const_iterator const & current;

	template <typename T>
	T soci_type(T*);

	long soci_type(short*);
	long soci_type(int*);
	unsigned long soci_type(unsigned short*);
	unsigned long soci_type(unsigned int*);
	double soci_type(float*);
	// TODO: there are probably more types that need to be
	// re-mapped like this
public:
	soci_fact_data(rowset::const_iterator const & iter)
	 : current(iter)
	{ }

	inline int col_count(void) const
	{
		return current->size();
	}

	inline bool is_ok(int col_index) const
	{
		return current->get_indicator(col_index) == soci::i_ok;
	}

	inline std::string get_name(int col_index) const
	{
		return current->get_properties(col_index).get_name();
	}

	inline int find_column(const std::string& name, int guess) const
	{
		std::size_t i=0, n=col_count();
		while(i != n)
		{
			int x = (i + guess) % n;
			if(get_name(x) == name) return x;
			++i;
		}
		return -1;
	}

	template <typename T>
	inline T get_as(int col_index)
	{
		return current->get<decltype(soci_type((T*)0))>(col_index);
	}
};

typedef factory_maker<
	sql_fact_source,
	sql_fact_pool,
	sql_fact_enum,
	sql_fact_def_traits<sql_fact_helper, soci_fact_data>
> soci_factory_maker;

typedef factory_maker<
	sql_fact_source,
	sql_fact_pool,
	sql_fact_enum,
	sql_fact_def_traits<sql_fact_quick_helper, soci_fact_data>
> soci_quick_factory_maker;

MIRROR_NAMESPACE_END

#endif //include guard

