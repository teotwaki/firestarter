
#ifndef __MODULEMANAGER_H
#define __MODULEMANAGER_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include <libconfig.h++>
#include <string>
#include <list>
#include <exception>
#include <boost/tr1/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>

#if HAVE_LTDL_H
	#include <ltdl.h>
#endif

#include "log.h"
#include "module.h"

namespace firestarter {
	namespace ModuleManager {

DECLARE_LOG(modManLog, "ModuleManager");

#if HAVE_LTDL_H
typedef boost::tuple<std::string, libconfig::Config &, int, lt_dlhandle, create_module *, destroy_module *> ModuleInfo;
/*             |     ^- path      ^- module config     |    |            |                |                 ^- Type name
               ^- tuple                                |    |            |                ^- delete function pointer
                                                       |    |            ^- factory function pointer
                                                       |    ^- shared library file handle
                                                       ^- module version
*/
#endif

typedef boost::unordered_map<std::string, ModuleInfo &> ModuleMap;
/*             ^- hash map   |            |             ^- Type name
                             |            ^- module info
                             ^- key (module name)
*/

typedef boost::unordered_map<std::string, std::list<std::string> > ModuleDependencyMap;
/*             ^- hash map   |            |                        ^- Type name
                             |            ^- list of dependencies' names
                             ^- key (module name)
*/

class ModuleManager {

	private:
	ModuleMap modules;
	ModuleDependencyMap dependencies;
	const libconfig::Config & configuration;
	std::string module_path;
	int ltdl;

	public:
	ModuleManager(const libconfig::Config & config);
	~ModuleManager();
	void loadModule(const std::string & name);
	ModuleInfo & getModule(const std::string & name);

};

/* Closing the namespace */
	}
}

namespace firestarter {
	namespace exception {

class ModuleNotFoundException : public std::exception {
	virtual const char * what() const throw() {
		return "Requested module could not be found";
	}
};

class ModuleNotLoadableException : public ModuleNotFoundException {
	virtual const char * what() const throw() {
		return "Requested module could not be loaded";
	}
};

class MissingDependencyException : public ModuleNotLoadableException {
	virtual const char * what() const throw() {
		return "Module dependency could not be found";
	}
};

/* Closing the namespace */
	}
}
#endif
