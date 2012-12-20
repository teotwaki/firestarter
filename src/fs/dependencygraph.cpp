#include "dependencygraph.hpp"

#ifndef logger
namespace firestarter { namespace ModuleManager { namespace DependencyGraph {
	DECLARE_LOG(logger, "firestarter.ModuleManager.DependencyGraph");
} } }
#endif

using namespace firestarter::ModuleManager::DependencyGraph;

DependencyGraph::DependencyGraph() : SimpleCache<std::list<std::string> >() {
	this->vertices["root"] = boost::add_vertex(std::string("root"), this->graph);
}

DependencyGraph::~DependencyGraph() {
	this->invalidateCache();
}

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

std::list<std::string> * DependencyGraph::resolve() {
	LOG_INFO(logger, "Attempting to resolve the dependency graph.");

	if (this->cacheIsValid()) {
		LOG_DEBUG(logger, "Returning cached results.");
		return this->getCache();
	}

	boost::topological_sort(graph, std::back_inserter(this->dependencies));
	return this->getModules();
}

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

