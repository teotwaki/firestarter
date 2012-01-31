
#ifndef __MODULEMANAGER_H
#define __MODULEMANAGER_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.h"
#include "dependencygraph.h"

#include <libconfig.h++>
#include <list>
#include <exception>
#include <boost/tr1/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/string.hpp>

#include <ltdl.h>

namespace firestarter {
	namespace ModuleManager {

class ModuleInfo {

	private:
	libconfig::Config * configuration;
	int version;
	lt_dlhandle handle;
	create_module * factory;
	destroy_module * recycling_facility;

	public:
	inline libconfig::Config * getConfiguration() { return this->configuration; };
	inline int getVersion() { return this->version; };
	inline lt_dlhandle getHandle() { return this->handle != NULL ? this->handle : NULL; };
	inline Module * instantiate() { return this->factory != NULL ? this->factory() : NULL; };
	inline void destroy(Module * module) { if (this->recycling_facility != NULL) this->recycling_facility(module); };

	ModuleInfo() : factory(NULL), recycling_facility(NULL) { };
	inline void setConfiguration(libconfig::Config * configuration) { this->configuration = configuration; };
	inline void setHandle(const lt_dlhandle handle) { this->handle = handle; };
	inline void setVersion(int version) { this->version = version; };
	inline void setFactory(create_module * factory) { this->factory = factory; };
	inline void setRecyclingFacility(destroy_module * recycling_facility) { this->recycling_facility = recycling_facility; };
	
};

typedef boost::unordered_map<std::string, ModuleInfo *> ModuleMap;
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
	lt_dladvise advise;
	DependencyGraph::DependencyGraph dependencies;

	public:
	ModuleManager(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	~ModuleManager();
	void loadModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	void loadModules();
	void lookupDependencies(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	libconfig::Config * loadModuleConfiguration(const std::string & module_name);
	ModuleInfo * getModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	inline ModuleMap & getModuleList() { return this->modules; }
	inline bool is_initialised() { return not (this->ltdl != 0); }
	inline std::string getModulePath() { return this->module_path; }

};

/* Closing the namespace */
	}
}

#endif
