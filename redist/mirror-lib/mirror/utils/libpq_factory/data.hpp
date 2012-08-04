/**
 * .file mirror/utils/libpq_factory/data.hpp
 * .brief Class containing data for the libpq-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LIBPQ_FACTORY_DATA_1011291729_HPP
#define MIRROR_UTILS_LIBPQ_FACTORY_DATA_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/lex_conv.hpp>
#include <libpq-fe.h>

MIRROR_NAMESPACE_BEGIN

class libpq_fact_data
{
private:
	PGresult* pg_result;
	const int& row_ref;
public:
	inline libpq_fact_data(PGresult* res, const int& row)
	 : pg_result(res)
	 , row_ref(row)
	{ }

	inline int col_count(void) const
	{
		return ::PQnfields(pg_result);
	}

	inline bool is_ok(int col_index) const
	{
		return ::PQgetisnull(pg_result, row_ref, col_index) == 0;
	}

	inline std::string get_name(int col_index) const
	{
		return ::PQfname(pg_result, col_index);
	}

	inline int find_column(const std::string& name, int) const
	{
		return ::PQfnumber(pg_result, name.c_str());
	}

	template <typename T>
	inline T get_as(int col_index)
	{
		lex_conv<std::string, T> convert_from_str;
		return convert_from_str(
			::PQgetvalue(
				pg_result,
				row_ref,
				col_index
			)
		);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

