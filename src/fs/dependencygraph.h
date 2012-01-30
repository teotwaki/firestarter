#ifndef __DEPENDENCYGRAPH_H
#define __DEPENDENCYGRAPH_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include <list>
#include <exception>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

#include "helper.h"

namespace firestarter {
	namespace ModuleManager {
		namespace DependencyGraph {

/* Graph for dependency resolution */
typedef boost::adjacency_list< boost::listS, // Store all out edges as an std::vector
                               boost::vecS, // Store all vertices in an std::vector
                               boost::directedS, // Relationships can go both ways
                               boost::property<boost::vertex_name_t, std::string> // Vertices have a name
                             > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::vector<Vertex> ModuleDependencyContainer;
typedef boost::unordered_map<std::string, Graph::vertex_descriptor> VertexMap;

class DependencyGraph {

	private:
	Graph graph;
	VertexMap vertices;
	ModuleDependencyContainer dependencies;

	public:
	DependencyGraph();
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
