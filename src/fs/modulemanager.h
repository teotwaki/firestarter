
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
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

#include <ltdl.h>

#include "helper.h"

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

/* Graph for dependency resolution */
typedef boost::adjacency_list< boost::listS, // Store all out edges as an std::vector
                               boost::vecS, // Store all vertices in an std::vector
                               boost::undirectedS, // Relationships can go both ways
                               boost::property<boost::vertex_name_t, std::string> // Vertices have a name
                             > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::vector<Vertex> ModuleDependencyContainer;
typedef boost::unordered_map<std::string, Graph::vertex_descriptor> VertexMap;

class ModuleManager {

	private:
	ModuleMap modules;
	ModuleDependencyContainer dependencies;
	const libconfig::Config & configuration;
	std::string module_path;
	int ltdl;

	public:
	ModuleManager(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException);
	~ModuleManager();
	void loadModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	void loadModules();
	ModuleInfo & getModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	inline ModuleMap & getModuleList() { return this->modules; }
	inline bool is_initialised() { return not (this->ltdl != 0); }
	inline std::string getModulePath() { return this->module_path; }

};

/* Closing the namespace */
	}
}

#endif
