/*
 * Copyright (C) 2012  Sebastian Lauwers <sebastian.lauwers@gmail.com>
 *
 * This file is part of Firestarter.
 *
 * Firestarter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firestarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRESTARTER_PERSISTENT_STORAGE_HPP
#define FIRESTARTER_PERSISTENT_STORAGE_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#ifdef HAVE_CONFIG_H
  #include "config.h"

  #ifdef HAVE_SOCI_SQLITE
    #include "vendor/sqlite/sqlite.hpp"
  #endif

  #ifdef HAVE_SOCI_MYSQL
    #include "vendor/mysql/mysql.hpp"
  #endif

  #ifdef HAVE_SOCI_PGSQL
    #include "vendor/postgresql/postgresql.hpp"
  #endif
#endif

namespace firestarter {
	namespace common {
		namespace Persistent {

	static __thread soci::session sql;
	static __thread boost::shared_ptr<soci::statement> statement;
	static __thread unsigned int counter;

	class Storage {
		public:
		static boost::shared_ptr<soci::statement> getStatement() {
			if (!statement)
				statement = boost::shared_ptr<soci::statement>(new soci::statement(sql));
				
			return statement;
		};

		static inline void connect(std::string const & connection_string) {
			sql.open(connection_string);
		};

		static inline void resetStatement() {
			statement.reset();
			counter = 0;
		};
	};

		}
	}
}

#endif
