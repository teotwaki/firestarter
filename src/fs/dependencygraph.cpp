#include "dependencygraph.h"

#ifndef logger
namespace firestarter { namespace ModuleManager {
	DECLARE_EXTERN_LOG(logger);
} }
#endif

using namespace firestarter::ModuleManager;

DependencyGraph::DependencyGraph() {
	this->vertices["root"] = boost::add_vertex(std::string("root"), this->graph);
}

void DependencyGraph::addDependency(const std::string & child_name, const std::string & parent_name) {

	if (this->vertices.find(parent_name) == this->vertices.end()) {
		/** \todo Throw exception if the parent module does not exist in the list of vertices */
	}

	if (this->vertices.find(child_name) == this->vertices.end())
		this->vertices[child_name] = boost::add_vertex(child_name, this->graph);

	boost::add_edge(this->vertices.at(parent_name), this->vertices.at(child_name), this->graph);
}

void DependencyGraph::removeDependency(const std::string & child_name, const std::string & parent_name) {

	if (this->vertices.find(parent_name) == this->vertices.end()) {
		/** \todo Throw exception if the parent module does not exist in the list of vertices */
	}

	if (this->vertices.find(child_name) == this->vertices.end()) {
		/** \todo Throw exception if the child module does not exist in the list of vertices */
	}

	boost::remove_edge(this->vertices.at(parent_name), this->vertices.at(child_name), this->graph);
}

std::list<std::string> * DependencyGraph::resolve() {
	boost::topological_sort(graph, std::back_inserter(this->dependencies));
	return this->getModuleMap();
}

std::list<std::string> * DependencyGraph::getModuleMap() {

	boost::property_map<Graph, boost::vertex_name_t>::type module_names = boost::get(boost::vertex_name, this->graph);
	std::list<std::string> * modules = new std::list<std::string>;

	for (ModuleDependencyContainer::iterator dependency = this->dependencies.begin();
	     dependency != this->dependencies.end(); 
	     dependency++) {

		LOG_DEBUG(logger, "Topological ordering of dependencies:");
		LOG_DEBUG(logger, " - " << module_names[*dependency]);

		modules->push_back(module_names[*dependency]);
	}

	return modules;
}
