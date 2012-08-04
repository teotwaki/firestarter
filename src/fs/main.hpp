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

#ifndef FIRESTARTER_MAIN_HPP
#define FIRESTARTER_MAIN_HPP

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.hpp"
#include "modulemanager.hpp"
#include "instancemanager.hpp"

#include <libconfig.h++>
#include <iostream>
#include <vector>
#include <boost/tr1/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#if HAVE_LTDL_H
	#include <ltdl.h>
#endif

using namespace std;

#endif
