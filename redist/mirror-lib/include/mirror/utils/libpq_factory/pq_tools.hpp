/**
 * .file mirror/utils/libpq_factory/pq_tools.hpp
 * .brief Simple helper wrappers for things from libpq
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LIBPQ_FACTORY_PQ_TOOLS_1011291729_HPP
#define MIRROR_UTILS_LIBPQ_FACTORY_PQ_TOOLS_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/libpq_factory/data.hpp>
#include <libpq-fe.h>
#include <stdexcept>

MIRROR_NAMESPACE_BEGIN

class libpq_fact_result
{
private:
	PGresult* res;
	int row;

	friend class libpq_fact_db;

	libpq_fact_result(PGresult* r, ExecStatusType ok_status)
	 : res(r)
	 , row(0)
	{
		if(PQresultStatus(res) != ok_status)
			throw std::runtime_error(
				PQresultErrorMessage(res)
			);
	}
public:
	libpq_fact_result(const libpq_fact_result&) = delete;

	libpq_fact_result(libpq_fact_result&& other)
	 : res(other.res)
	 , row(other.row)
	{
		other.res = nullptr;
	}

	~libpq_fact_result(void)
	{
		if(res != nullptr) PQclear(res);
	}

	libpq_fact_data data(void)
	{
		return libpq_fact_data(res, row);
	}

	int position(void) const
	{
		return row;
	}

	bool empty(void) const
	{
		return row >= PQntuples(res);
	}

	libpq_fact_result& next(void)
	{
		++row;
		return *this;
	}
};

class libpq_fact_db
{
private:
	PGconn* conn;
public:
	libpq_fact_db(const std::string& conn_para)
	 : conn(PQconnectdb(conn_para.c_str()))
	{
		if(PQstatus(conn) != CONNECTION_OK)
			throw std::runtime_error(
				PQerrorMessage(conn)
			);
	}

	libpq_fact_db(const libpq_fact_db&) = delete;

	~libpq_fact_db(void)
	{
		if(conn != nullptr) PQfinish(conn);
	}

	libpq_fact_result query(const std::string& qstr)
	{
		return libpq_fact_result(
			PQexec(conn, qstr.c_str()),
			PGRES_TUPLES_OK
		);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

