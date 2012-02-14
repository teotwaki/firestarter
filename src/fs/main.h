
#ifndef __MAIN_H
#define __MAIN_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.h"
#include "modulemanager.h"
#include "instancemanager.h"

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
