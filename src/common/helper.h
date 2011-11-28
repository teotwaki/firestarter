#ifndef __HELPER_H
#define __HELPER_H

#include <string>
#include <sys/stat.h>

#include "log.h"
#include "module.h"

// Prettify BOOST's foreach.
#ifndef foreach
	#define foreach BOOST_FOREACH
#endif
#ifndef reverse_foreach
	#define reverse_foreach BOOST_FOREACH_REVERSE
#endif

bool file_exists(std::string path);

#endif
