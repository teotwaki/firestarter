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

#ifndef FIRESTARTER_MYSQL_HPP
#define FIRESTARTER_MYSQL_HPP

#include "config.h"

#ifndef HAVE_SOCI_MYSQL
	#error MYSQL libraries were not found during ./configure. You can not use this backend. \
Install the MySQL development libraries, and ensure SOCI-MySQL is properly installed.
#endif

#include <soci/soci.h>
#include <soci/mysql/common.h>

#endif
