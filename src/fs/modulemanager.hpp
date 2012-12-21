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

#ifndef FIRESTARTER_MODULEMANAGER_HPP
#define FIRESTARTER_MODULEMANAGER_HPP

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.hpp"
#include "simplecache.hpp"
#include "dependencygraph.hpp"
#include "zmq/zmqhelper.hpp"
#include "module.hpp"

#include <libconfig.h++>
#include <list>
#include <boost/tr1/unordered_map.hpp>
#include <boost/algorithm/string.hpp>

#include <ltdl.h>
#ifdef LTDL_DEBUG
	// We need dlopen() and dlerror() in order to get around the lt_dlopen bug that prevents us
	// from knowing why a module didn't load.
	#include <dlfcn.h>
#endif

namespace firestarter {
	namespace ModuleManager {

using namespace firestarter::module;

/** \brief Encapsulates module information
  *
  * The ModuleInfo class encapsulates all the different aspects of a module (as seen from the ModuleManager class' perspective)
  * in order to provide a unified way to access the data (for example, by abstracting ugly things like function pointers in instantiate(), etc.).
  * It also provides a very basic inspection ability implemented by isValid().
  *
  * This class is mainly used by ModuleManager, which is why it shares the header file.
  *
  * \see ModuleManager
  */
class ModuleInfo {

	private:
	/// \brief Pointer to the module's configuration object
	libconfig::Config * configuration;
	/// \brief Module version (not really used yet)
	int version;
	/// \brief Handle to the shared library object file
	lt_dlhandle handle;
	/// \brief Function pointer to the factory function
	create_module * factory;
	/// \brief Function pointer to the descructor function
	destroy_module * recycling_facility;
	/// \brief The ZMQ Context to use when instantiating modules
	zmq::context_t * context;

	public:
	/// \brief Constructs the class to a ready-to-use state
	ModuleInfo() : configuration(NULL), version(0), handle(NULL), factory(NULL), recycling_facility(NULL), context(NULL) { };
	/// \brief Returns a pointer to the configuration object
	inline libconfig::Config * getConfiguration() { return this->configuration; };
	inline int getVersion() { return this->version; };
	inline lt_dlhandle getHandle() { return this->handle; };
	/** \brief Instantiates the module
	  *
	  * This method returns an instance of the module as returned by the create__MODULENAME__ function in the shared library.
	  * It is implemented by simply calling the function pointer. No error checking is made.
	  *
	  * This pointer needs to be passed back to destroy() for proper deletion, instead of calling delete on it.
	  */
	inline Module * instantiate() { return this->factory != NULL ? this->factory(*(this->context)) : NULL; };
	/** \brief Instantiates the module within a specific context
	  *
	  * Same as the regular instantiate() method, excepted that you can pass a specific ZMQ context instead.
	  */
	inline Module * instantiate(zmq::context_t & context) { return this->factory != NULL ? this->factory(context) : NULL; };
	/** \brief Call the destructor for the module
	  *
	  * This method provides a way to delete the module. It is not possible to directly call the destructor or delete,
	  * as this would call the Module baseclass destructor, rather than the actual instantiated destructor (and spawn memory leaks).
	  *
	  * \warning Please take care to correctly destroy instantiated modules.
	  */
	inline void destroy(Module * module) { if (this->recycling_facility != NULL) this->recycling_facility(module); };

	inline void setConfiguration(/** [in] */ libconfig::Config * configuration) { this->configuration = configuration; };
	inline void setHandle(/** [in] */ const lt_dlhandle handle) { this->handle = handle; };
	inline void setVersion(/** [in] */ int version) { this->version = version; };
	inline void setFactory(/** [in] */ create_module * factory) { this->factory = factory; };
	inline void setRecyclingFacility(/** [in] */ destroy_module * recycling_facility) { this->recycling_facility = recycling_facility; };
	inline void setContext(/** [in] */ zmq::context_t & context) { this->context = &context; };

	/** \brief Check if the module seems valid
	  *
	  * The isValid() method provides a very basic check to see if all the components are correctly initialised. It does not do any complex
	  * analysis, and can for example, return true even though some of the data structures point to discarded or invalid memory.
	  */
	inline bool isValid() {
		if (this->configuration != NULL && this->handle != NULL &&
				this->factory != NULL && this->recycling_facility != NULL)
			return true;

		return false;
	};

	/** \brief Check if the module needs to be auto-started
	  *
	  * \return The value of the module.autostart configuration key or true if it can't be found
	  */
	inline bool shouldAutostart() {
		return this->getConfiguration()->exists("module.autostart") ?
			this->getConfiguration()->lookup("module.autostart") : true;
	};

	/** \brief Check if the module wants to run in its own thread
	  *
	  * \return The value of the module.threaded configuration key or false if it can't be found
	  */
	inline bool shouldRunThreaded() {
		return this->getConfiguration()->exists("module.threaded") ?
			static_cast<bool>(this->getConfiguration()->lookup("module.threaded")) : false;
	};

	/** \brief Check if the module wants to run in its own process
	  *
	  * \return The value of the module.standalone configuration key or false if it can't be found
	  */
	inline bool shouldRunStandAlone() {
		return this->getConfiguration()->exists("module.standalone") ?
			static_cast<bool>(this->getConfiguration()->lookup("module.standalone")) : false;
	};
	
};

/// ModuleInfo hashmap with the module's name as key
typedef boost::unordered_map<std::string, ModuleInfo *> ModuleMap;
/*             ^- hash map   |            |           ^- Type name
                             |            ^- module info
                             ^- key (module name)
*/

/** \brief Manages the module's loading priority
  *
  * The ModuleManager class abstracts the complexities of having to manually open the shared libraries, configuration files
  * and manually track the modules' respective dependencies. The only thing it needs is a reference to the application's configuration
  * file. Its constructor will do most of the heavy lifting (read the config, resolve the dependencies, load the modules, etc),
  * even though convenience methods are available should modules be loaded after the initial constructor run.
  *
  * This class relies on DependencyGraph and ModuleInfo.
  *
  * \see DependencyGraph
  * \see ModuleInfo
  */
class ModuleManager {

	private:
	/// \brief Contains the currently loaded modules
	ModuleMap modules;
	/// \brief Reference to the application configuration
	const libconfig::Config & configuration;
	std::string module_path;
	/// \brief libltdl status flag
	int ltdl;
	/// \brief advise object passed to ltdl during module loading
	lt_dladvise advise;
	/// \brief Dependency graph used for dependency resolution.
	DependencyGraph::DependencyGraph graph;

	public:
	ModuleManager(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	~ModuleManager();
	void loadModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	void loadModules();
	void lookupDependencies(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	libconfig::Config * loadModuleConfiguration(const std::string & module_name);
	ModuleInfo * getModuleInfo(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	inline std::list<std::string> * getModuleList() { return this->graph.getModules(); }
	inline bool isInitialised() { return not (this->ltdl != 0); }
	inline std::string getModulePath() { return this->module_path; }

};

/* Closing the namespace */
	}
}

#endif
