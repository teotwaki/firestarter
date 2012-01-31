#include "dependencygraph.h"

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
DependencyGraph::DependencyGraph() : cached(false) {
	this->vertices["root"] = boost::add_vertex(std::string("root"), this->graph);
}

/** \brief Destroys the object
  *
  * The destructor deletes the cache if it exists.
  */
DependencyGraph::~DependencyGraph() {
	if (this->modules != NULL)
		delete this->modules;
}

/** \brief Add a module to the graph
  *
  * Add a dependency to the graph. An exception is thrown if the second argument can not be found in the graph.
  */
void DependencyGraph::addDependency(/** [in] */ const std::string & child_name, /** [in] */ const std::string & parent_name) {

	LOG_INFO(logger, "Storing `" << child_name << "' to be used by `" << parent_name << "'.");

	if (this->vertices.find(parent_name) == this->vertices.end()) {
		LOG_ERROR(logger, "Parent module `" << parent_name << "' does not exist in list of vertices.");
		/// \todo Throw exception if the parent module does not exist in the list of vertices
	}

	if (this->vertices.find(child_name) == this->vertices.end()) {
		LOG_DEBUG(logger, "Adding module `" << child_name << "' to list of vertices.");
		this->vertices[child_name] = boost::add_vertex(child_name, this->graph);
	}

	LOG_DEBUG(logger, "Adding edge from child module `" << child_name << "' to parent module `" << parent_name << "'.");
	boost::add_edge(this->vertices.at(child_name), this->vertices.at(parent_name), this->graph);
}

/** \brief Remove a dependency from the graph
  *
  * As the method's name indicates, it removes a dependency from the graph. If the first or second parameter can't be found
  * in the graph, an exception is thrown (or will be, in the future).
  */
void DependencyGraph::removeDependency(/** [in] */ const std::string & child_name, /** [in] */ const std::string & parent_name) {

	LOG_INFO(logger, "Removing `" << child_name << "' from `" << parent_name << "'.");

	if (this->vertices.find(parent_name) == this->vertices.end()) {
		LOG_ERROR(logger, "Parent module `" << parent_name << "' does not exist in the list of vertices.");
		/// \todo Throw exception if the parent module does not exist in the list of vertices
	}

	if (this->vertices.find(child_name) == this->vertices.end()) {
		LOG_ERROR(logger, "Parent module `" << child_name << "' does not exist in the list of vertices.");
		/// \todo Throw exception if the child module does not exist in the list of vertices 
	}

	LOG_DEBUG(logger, "Removing edge from parent module `" << parent_name << "' to child module `" << child_name << "'.");
	boost::remove_edge(this->vertices.at(parent_name), this->vertices.at(child_name), this->graph);
}


/** \brief Sort the graph topologically
  *
  * This method applies boost::topological_sort to the stored graph.
  * After sorting the graph, it invalidates the cache.
  *
  * \return The cache computed by getModules()
  * \see getModules
  */
std::list<std::string> * DependencyGraph::resolve() {
	LOG_INFO(logger, "Attempting to resolve the dependency graph.");
	boost::topological_sort(graph, std::back_inserter(this->dependencies));
	LOG_DEBUG(logger, "Invalidating cache.");
	this->cached = false;
	return this->getModules();
}

/** \brief Obtain the order in which the modules should be loaded to avoid dependency issues.
  *
  * The getModules method extracts from the (previously resolve()'d) Graph the list of modules in the order in which they should be loaded.
  * The order is important, as it enables specific modules to provide symbols that other modules require.
  *
  * A small level of caching is implemented: Once returned, the pointer to the list will remain the same as long as resolve() is not called
  * (and hence, the cache invalidated). This being said, the cache is only invalidated upon calling getModules(). So this for example is valid:
  * \code
  * DependencyGraph graph;
  * graph.addDependency("bar");
  * graph.addDependency("foo", "bar");
  * std::list<std::string> * module_order = graph.resolve(); // getModules() is called by resolve()
  * graph.addDependency("taz", "bar");
  * // Here, even though the graph has changed, the module_order pointer is still valid
  * std::list<std::string> * first = graph.resolve(); // module_order now points to invalid memory
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

	if (cached) {
		LOG_DEBUG(logger, "Returning cached results");
		return this->modules;
	}

	else if (this->modules != NULL) {
		LOG_DEBUG(logger, "Cache is out-of-date. Deleting.");
		delete this->modules;
		this->modules = NULL;
	}

	boost::property_map<Graph, boost::vertex_name_t>::type module_names = boost::get(boost::vertex_name, this->graph);
	
	LOG_DEBUG(logger, "Creating new cache.");
	std::list<std::string> * modules = new std::list<std::string>;

	LOG_DEBUG(logger, "Populating cache.");
	for (ModuleDependencyContainer::reverse_iterator dependency = this->dependencies.rbegin();
	     dependency != this->dependencies.rend(); 
	     dependency++) {
		if (module_names[*dependency] != "root") {
			LOG_DEBUG(logger, "Adding `" << module_names[*dependency] << "' to cache.");
			modules->push_back(module_names[*dependency]);
		}
	}

	LOG_DEBUG(logger, "Setting cache as valid.");
	this->cached = true;

	LOG_DEBUG(logger, "Returning cache.");
	return modules;
}
