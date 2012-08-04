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

#include "dependencygraph.hpp"

#ifndef logger
namespace firestarter { namespace ModuleManager { namespace DependencyGraph {
	DECLARE_LOG(logger, "firestarter.ModuleManager.DependencyGraph");
} } }
#endif

using namespace firestarter::ModuleManager::DependencyGraph;

/** \brief Initialises a DependencyGraph
  *
  * This constructor initialises the object so that it can be used immediately.
  */
DependencyGraph::DependencyGraph() : SimpleCache<std::list<std::string> >() {
	this->vertices["root"] = boost::add_vertex(std::string("root"), this->graph);
}

/** \brief Destroys the object
  *
  * The destructor deletes the cache if it exists.
  */
DependencyGraph::~DependencyGraph() {
	this->invalidateCache();
}

/** \brief Add a module to the graph
  *
  * Add a dependency to the graph. An exception is thrown if the second argument can not be found in the graph.
  *
  * \warning If it exists, the getModules() cache is invalidated.
  */
void DependencyGraph::addDependency(/** [in] */ const std::string & child_name, 
                                    /** [in] */ const std::string & parent_name) {

	LOG_INFO(logger, "Storing `" << child_name << "' to be used by `" << parent_name << "'.");

	if (this->vertices.find(parent_name) == this->vertices.end()) {
		LOG_ERROR(logger, "Parent module `" << parent_name << "' does not exist in list of vertices.");
		/// \todo Throw exception if the parent module does not exist in the list of vertices
	}

	if (this->vertices.find(child_name) == this->vertices.end()) {
		LOG_DEBUG(logger, "Adding module `" << child_name << "' to list of vertices.");
		this->vertices[child_name] = boost::add_vertex(child_name, this->graph);
	}

	LOG_DEBUG(logger, 
		"Adding edge from child module `" << child_name << "' to parent module `" << parent_name << "'.");
	boost::add_edge(this->vertices.at(child_name), this->vertices.at(parent_name), this->graph);

	this->invalidateCache();
}

/** \brief Remove a dependency from the graph
  *
  * As the method's name indicates, it removes a dependency from the graph. If the first or second parameter can't be
  * found in the graph, an exception is thrown (or will be, in the future).
  * 
  * \warning If it exists, the getModules() cache is invalidated.
  */
void DependencyGraph::removeDependency(/** [in] */ const std::string & child_name, 
                                       /** [in] */ const std::string & parent_name) {

	LOG_INFO(logger, "Removing `" << child_name << "' from `" << parent_name << "'.");

	if (this->vertices.find(parent_name) == this->vertices.end()) {
		LOG_ERROR(logger, "Parent module `" << parent_name << "' does not exist in the list of vertices.");
		/// \todo Throw exception if the parent module does not exist in the list of vertices
	}

	if (this->vertices.find(child_name) == this->vertices.end()) {
		LOG_ERROR(logger, "Parent module `" << child_name << "' does not exist in the list of vertices.");
		/// \todo Throw exception if the child module does not exist in the list of vertices 
	}

	LOG_DEBUG(logger,
		"Removing edge from parent module `" << parent_name << "' to child module `" << child_name << "'.");
	boost::remove_edge(this->vertices.at(parent_name), this->vertices.at(child_name), this->graph);

	this->invalidateCache();
}


/** \brief Sort the graph topologically
  *
  * This method applies boost::topological_sort to the stored graph. The graph is only sorted when the cache is
  * invalid.
  *
  * \return The cache computed by getModules()
  * \see getModules
  */
std::list<std::string> * DependencyGraph::resolve() {
	LOG_INFO(logger, "Attempting to resolve the dependency graph.");

	if (this->cacheIsValid()) {
		LOG_DEBUG(logger, "Returning cached results.");
		return this->getCache();
	}

	boost::topological_sort(graph, std::back_inserter(this->dependencies));
	return this->getModules();
}

/** \brief Obtain the order in which the modules should be loaded to avoid dependency issues.
  *
  * The getModules method extracts from the (previously resolve()'d) Graph the list of modules in the order in which
  * they should be loaded. The order is important, as it enables specific modules to provide symbols that other modules
  * require.
  *
  * A small level of caching is implemented: Once returned, the pointer to the list will remain the same as long as
  * addDependency() or removeDependency() is not called (and hence, the cache invalidated). For example:
  * \code
  * DependencyGraph graph;
  * graph.addDependency("bar");
  * graph.addDependency("foo", "bar");
  * std::list<std::string> * module_order = graph.resolve(); // getModules() is called by resolve(), cache is init'd
  * graph.addDependency("taz", "bar"); // cache is invalidated, module_order should not be used anymore
  * std::list<std::string> * first = graph.resolve(); // cache is initialised
  * std::list<std::string> * second = graph.getModules(); // first == second
  * \endcode 
  *
  * \return Pointer to a list of strings.
  * This pointer is managed, which means it should not be deleted by the receiver.
  * If the graph has not been resolve()'d before calling getModules(), the value returned will point to an empty list.
  *
  * \see resolve
  */
std::list<std::string> * DependencyGraph::getModules() {

	if (this->cacheIsValid()) {
		LOG_DEBUG(logger, "Returning cached results");
		return this->getCache();
	}

	this->initCache();

	boost::property_map<Graph, boost::vertex_name_t>::type module_names = boost::get(boost::vertex_name, this->graph);
	
	LOG_DEBUG(logger, "Populating cache.");
	for (ModuleDependencyContainer::reverse_iterator dependency = this->dependencies.rbegin();
	     dependency != this->dependencies.rend(); 
	     dependency++) {
		if (module_names[*dependency] != "root") {
			LOG_DEBUG(logger, "Adding `" << module_names[*dependency] << "' to cache.");
			this->getCache()->push_back(module_names[*dependency]);
		}
	}

	LOG_DEBUG(logger, "Returning cache.");
	return this->getCache();
}

