#ifndef __FOREACH_H
#define __FOREACH_H

// Prettify BOOST's foreach.
#ifndef foreach
	#define foreach BOOST_FOREACH
#endif
#ifndef reverse_foreach
	#define reverse_foreach BOOST_FOREACH_REVERSE
#endif

// There is a bug in boost 1.48 which requires foreach to be defined
// before including foreach.hpp

#include <boost/foreach.hpp>

#endif
