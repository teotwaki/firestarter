
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
