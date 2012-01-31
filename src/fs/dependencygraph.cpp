#include "dependencygraph.h"

#ifndef logger
namespace firestarter { namespace ModuleManager { namespace DependencyGraph {
	DECLARE_LOG(logger, "firestarter.ModuleManager.DependencyGraph");
} } }
#endif

using namespace firestarter::ModuleManager::DependencyGraph;

DependencyGraph::DependencyGraph() : cached(false) {
	this->vertices["root"] = boost::add_vertex(std::string("root"), this->graph);
}

DependencyGraph::~DependencyGraph() {
	if (this->modules != NULL)
		delete modules;
}

void DependencyGraph::addDependency(const std::string & child_name, const std::string & parent_name) {

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

void DependencyGraph::removeDependency(const std::string & child_name, const std::string & parent_name) {

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

std::list<std::string> * DependencyGraph::resolve() {
	LOG_INFO(logger, "Attempting to resolve the dependency graph.");
	boost::topological_sort(graph, std::back_inserter(this->dependencies));
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

	if (cached)
		return this->modules;

	else if (this->modules != NULL) {
		delete this->modules;
		this->module = NULL;
	}

	boost::property_map<Graph, boost::vertex_name_t>::type module_names = boost::get(boost::vertex_name, this->graph);
	
	std::list<std::string> * modules = new std::list<std::string>;

	LOG_DEBUG(logger, "Topological ordering of dependencies:");

	for (ModuleDependencyContainer::reverse_iterator dependency = this->dependencies.rbegin();
	     dependency != this->dependencies.rend(); 
	     dependency++) {
		if (module_names[*dependency] != "root") {
			LOG_DEBUG(logger, "- " << module_names[*dependency]);
			modules->push_back(module_names[*dependency]);
		}
	}

	this->cached = true;

	return modules;
}
