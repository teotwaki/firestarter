#include "instancemanager.h"

#ifndef logger
namespace firestarter { namespace InstanceManager {
	DECLARE_LOG(logger, "firestarter.InstanceManager");
} }
#endif

using namespace firestarter::InstanceManager;

InstanceManager::InstanceManager(firestarter::ModuleManager::ModuleManager & modulemanager, zmq::context_t & context) throw(std::invalid_argument)
							: modulemanager(modulemanager), context(context), modules(context, ZMQ_REQ) {

	LOG_INFO(logger, "Constructing InstanceManager object");

	if (not modulemanager.isInitialised()) {
		LOG_ERROR(logger, "The ModuleManager object passed as argument isn't properly initialised.");
		throw std::invalid_argument("modulemanager");
	}

	LOG_DEBUG(logger, "Binding socket to manager endpoint (" << MANAGER_SOCKET_URI << ").");
	this->modules.bind(MANAGER_SOCKET_URI);

};

void InstanceManager::run(const std::string & name, bool autostart) throw(firestarter::exception::ModuleNotFoundException) {
	LOG_INFO(logger, "Attempting to run module `" << name << "'.")
	firestarter::ModuleManager::ModuleInfo * module_info = this->modulemanager.getModuleInfo(name);

	if (not module_info->isValid()) {
		LOG_ERROR(logger, "Module `" << name << "' is not quite ready to be loaded.");
		/// \todo Throw an exception instead of returning
		return;
	}

	if (autostart && not module_info->shouldAutostart()) {
		LOG_DEBUG(logger, "Module `" << name << "' does not want to be auto-started.");
		return;
	}

	if (module_info->shouldRunStandAlone()) {
		LOG_ERROR(logger, "Module `" << name << "' wants to run in its own process, but this hasn't been implemented yet.");
		/// \todo Throw an exception instead of returning
		/// \todo Implement process-spawning
		return;
	}

	if (module_info->shouldRunThreaded()) {
		using namespace firestarter::module;

		LOG_INFO(logger, "Spawning thread for module `" << name << "'.");
		RunnableModule * module = static_cast<RunnableModule *>(module_info->instantiate(this->context));
		boost::thread * thread = new boost::thread(&RunnableModule::run, module);
		this->threads[name] = std::make_pair(thread, module);
		return;
	}
	
	this->instances[name] = module_info->instantiate(this->context);
}

void InstanceManager::runAll(bool autostart) {
	LOG_INFO(logger, "Attempting to run all modules.");
	std::list<std::string> * module_list = this->modulemanager.getModuleList();

	foreach(std::string module_name, *module_list) {
		this->run(module_name, autostart);
	}
}

