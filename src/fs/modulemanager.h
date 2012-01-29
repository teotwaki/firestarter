
#ifndef __MODULEMANAGER_H
#define __MODULEMANAGER_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include <libconfig.h++>
#include <list>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/string.hpp>

#include <ltdl.h>

#include "helper.h"
#include "dependencygraph.h"

namespace firestarter {
	namespace ModuleManager {

typedef boost::tuple<libconfig::Config *, int, lt_dlhandle *, create_module *, destroy_module *> ModuleInfo;
/*             |     ^- module config     |    |              |                |                 ^- Type name
               ^- tuple                   |    |              |                ^- delete function pointer
                                          |    |              ^- factory function pointer
                                          |    ^- shared library file handle
                                          ^- module version
*/

typedef boost::unordered_map<std::string, ModuleInfo> ModuleMap;
/*             ^- hash map   |            |           ^- Type name
                             |            ^- module info
                             ^- key (module name)
*/

class ModuleManager {

	private:
	ModuleMap modules;
	const libconfig::Config & configuration;
	std::string module_path;
	int ltdl;
	DependencyGraph::DependencyGraph dependencies;

	public:
	ModuleManager(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	~ModuleManager();
	void loadModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	void loadModules();
	void lookupDependencies(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	libconfig::Config * loadModuleConfiguration(const std::string & module_name);
	ModuleInfo & getModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	inline ModuleMap & getModuleList() { return this->modules; }
	inline bool is_initialised() { return not (this->ltdl != 0); }
	inline std::string getModulePath() { return this->module_path; }

};

/* Closing the namespace */
	}
}

#endif
