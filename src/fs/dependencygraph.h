#ifndef __DEPENDENCYGRAPH_H
#define __DEPENDENCYGRAPH_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.h"

#include <list>
#include <exception>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace firestarter {
	namespace ModuleManager {
		namespace DependencyGraph {

/// Graph for dependency resolution
typedef boost::adjacency_list< boost::listS, ///< Store all out edges as an std::vector
                               boost::vecS, ///< Store all vertices in an std::vector
                               boost::directedS, ///< Relationships are unidirectional (a depends on b)
                               boost::property<boost::vertex_name_t, std::string> ///< Vertices have a name
                             > Graph;

/// \brief Enables access to a Vertex's properties
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
/// \brief Each Vertex represents a module in the Graph
typedef std::vector<Vertex> ModuleDependencyContainer;
/// \brief Hashmap of Vertex (modules) with their name as key
typedef boost::unordered_map<std::string, Graph::vertex_descriptor> VertexMap;

/** \brief Implements topological_sort from Boost.Graph.
  *
  * DependencyGraph enables dependency resolution through the use of the Boost.Graph library, and more specificaly the topological_sort algorithm.
  * It abstracts the complexity of using Boost.Graph by providing an easy to use interface. It is mainly used by ModuleManager.
  *
  * A small caching layer is implemented, to prevent useless graph sorting or list computation. 
  * The cache is invalidated when addDependency() or removeDependency() is called.
  *
  * \see ModuleManager
  */
class DependencyGraph {

	private:
	Graph graph;
	VertexMap vertices;
	ModuleDependencyContainer dependencies;
	std::list<std::string> * modules;

	void invalidateCache();
	void initCache();
	inline std::list<std::string> * getCache() { return this->modules; };
	inline void setCache(std::list<std::string> * cache) { this->modules = cache; };
	inline bool cacheIsValid() { return (this->getCache() != NULL) ? true : false; };

	public:
	DependencyGraph();
	~DependencyGraph();
	void addDependency(const std::string & child_name, const std::string & parent_name = "root");
	void removeDependency(const std::string & child_name, const std::string & parent_name = "root");
	std::list<std::string> * resolve();
	std::list<std::string> * getModules();

};

/* Closing the namespace */
		}
	}
}

#endif
