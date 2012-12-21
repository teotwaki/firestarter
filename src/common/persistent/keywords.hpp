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

#ifndef FIRESTARTER_PERSISTENT_KEYWORDS_HPP
#define FIRESTARTER_PERSISTENT_KEYWORDS_HPP

#include <mirror/ct_string.hpp>

namespace firestarter {
	namespace common {
		namespace Persistent {
			namespace keywords {

	struct select : mirror::cts::string<
		'S', 'E', 'L', 'E', 'C', 'T', ' '
	> { };

	struct from : mirror::cts::string<
		' ', 'F', 'R', 'O', 'M', ' '
	> { };

	struct or_ : mirror::cts::string<
		' ', 'O', 'R', ' '
	> { };
	
	struct and_ : mirror::cts::string<
		' ', 'A', 'N', 'D', ' '
	> { };

	struct delete_ : mirror::cts::string<
		'D', 'E', 'L', 'E', 'T', 'E', ' '
	> { };

	struct where : mirror::cts::string<
		' ', 'W', 'H', 'E', 'R', 'E', ' '
	> { };

	struct limit : mirror::cts::string<
		' ', 'L', 'I', 'M', 'I', 'T', ' '
	> { };

	struct order_by : mirror::cts::string<
		' ', 'O', 'R', 'D', 'E', 'R', ' ', 'B', 'Y', ' '
	> { };

	struct asc : mirror::cts::string<
		' ', 'A', 'S', 'C', ' '
	> { };

	struct desc : mirror::cts::string<
		' ', 'D', 'E', 'S', 'C', ' '
	> { };

	struct create_table : mirror::cts::string<
		'C', 'R', 'E', 'A', 'T', 'E', ' ', 'T', 'A', 'B', 'L', 'E', ' '
	> { };

	struct if_not_exists : mirror::cts::string<
		' ', 'I', 'F', ' ', 'N', 'O', 'T', ' ', 'E', 'X', 'I', 'S', 'T', 'S', ' '
	> { };

	struct insert_into : mirror::cts::string<
		'I', 'N', 'S', 'E', 'R', 'T', ' ', 'I', 'N', 'T', 'O', ' '
	> { };

	struct values : mirror::cts::string<
		' ', 'V', 'A', 'L', 'U', 'E', 'S', ' '
	> { };

	struct count : mirror::cts::string<
		' ', 'C', 'O', 'U', 'N', 'T', '(', '*', ')', ' '
	> { };

	struct update : mirror::cts::string<
		'U', 'P', 'D', 'A', 'T', 'E', ' '
	> { };

	struct set : mirror::cts::string<
		' ', 'S', 'E', 'T', ' '
	> { };

			}
		}
	}
}

#endif
