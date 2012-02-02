
#ifndef __MODULE_H
#define __MODULE_H

#include <string>
#include <boost/tr1/unordered_map.hpp>

#include "helper.h"

class Module {
	protected:
	/* Properties */

	public:
	virtual void setup() = 0; /* pure virtual */
};

typedef Module * create_module();
typedef void destroy_module(Module *);
typedef int module_version();

#endif
