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

#ifndef FIRESTARTER_DEPENDENCYGRAPH_HPP
#define FIRESTARTER_DEPENDENCYGRAPH_HPP

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.hpp"
#include "simplecache.hpp"

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
class DependencyGraph : private SimpleCache<std::list<std::string> > {

	private:
	Graph graph;
	VertexMap vertices;
	ModuleDependencyContainer dependencies;

	public:
	/** \brief Initialises a DependencyGraph
	 * 
	 * This constructor initialises the object so that it can be used immediately.
	 */
	DependencyGraph();

	/// \brief Destroys the object and deletes the cache if it was initiliased.
	~DependencyGraph();

	/** \brief Add a module to the graph.
	 *
	 * Add a dependency to the graph. An exception is thrown if the second argument can not be found
	 * in the graph.
	 *
	 * \warning If it exists, the getModules() cache is invalidated.
	 * \see getModules()
	 */
	void addDependency(const std::string & child_name, const std::string & parent_name = "root");

	/** \brief Remove a dependency from the graph.
	 *
	 * As the method's name indicates, it removes a dependency from the graph. If either parameter
	 * can not be found in the graph, an exception is thrown (or will be, in the future).
	 *
	 * \warning If it exists, the getModules() cache is invalidated.
	 * \see getModules()
	 */
	void removeDependency(const std::string & child_name, const std::string & parent_name = "root");

	/** \brief Sort the graph topologically.
	 *
	 * This method applies boost::topological_sort to the stored graph. The graph is only sorted when
	 * the cache is invalid. Once the graph is computed, the method calls getModules(), which initialises
	 * the cache and returns it.
	 */
	std::list<std::string> * resolve();

	/** Obtain the order in which the modules should be loaded to avoid dependency issues.
	 *
	 * The getModules method extracts from the (previously resolved()'d) Graph the list of modules, sorted
	 * in the order in which they should be loaded. The order is important, as it enables specific modules
	 * to provide symbols that other modules might require.
	 *
	 * A small level of caching is implemented: Once returned, the pointer to the list will remain the same
	 * as long as addDependency() or removeDependency() is not called (which invalidates the cache). For
	 * example:
	 * \code
	 * DependencyGraph graph;
	 * graph.addDependency("bar");
	 * graph.addDependency("foo", "bar");
	 * // getModules() is called by resolve(): cache is initialised.
	 * std::list<std::string> * module_order = graph.resolve();
	 * // cache is invalidated; module_order should not be used anymore.
	 * graph.addDependency("taz", "bar");
	 *
	 * std::list<std::string> * first = graph.resolve(); // cache is initialised
	 * std::list<std::string> * second = graph.getModules(); // first == second
	 * \endcode 
	 *
	 * \return Pointer to a list of strings.
	 * This pointer is managed, which means it should not be deleted by the receiver.
	 * If the graph has not been resolve()'d before calling getModules(), the method will return and empty list.
	 *
	 * \see resolve()
	 */

	std::list<std::string> * getModules();

};

/* Closing the namespace */
		}
	}
}

#endif
