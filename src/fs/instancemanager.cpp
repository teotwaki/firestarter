#include "instancemanager.h"

#ifndef logger
namespace firestarter { namespace InstanceManager {
	DECLARE_LOG(logger, "firestarter.InstanceManager");
} }
#endif

using namespace firestarter::InstanceManager;

InstanceManager::InstanceManager(firestarter::ModuleManager::ModuleManager & modulemanager, 
		zmq::context_t & context) throw(std::invalid_argument) : 
		modulemanager(modulemanager), context(context), socket(context), running(false), pending_modules(0) {

	LOG_INFO(logger, "Constructing InstanceManager object");

	if (not modulemanager.isInitialised()) {
		LOG_ERROR(logger, "The ModuleManager object passed as argument isn't properly initialised.");
		throw std::invalid_argument("modulemanager");
	}

};

void InstanceManager::run(const std::string & name, bool autostart) 
		throw(firestarter::exception::ModuleNotFoundException) {

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

	this->running = true;
	this->instances[name] = module_info->instantiate(this->context);

	if (module_info->shouldRunStandAlone()) {
		LOG_ERROR(logger, "Module `" << name << "' wants to run in its own process, but "
							"this hasn't been implemented yet.");
		/// \todo Throw an exception instead of returning
		/// \todo Implement process-spawning
		return;
	}

	if (module_info->shouldRunThreaded()) {
		using namespace firestarter::module;

		LOG_INFO(logger, "Spawning thread for module `" << name << "'.");
		RunnableModule * module = reinterpret_cast<RunnableModule *>(this->instances[name]);
		boost::thread * thread = new boost::thread(&RunnableModule::_initialiser, module);
		this->threads[name] = std::make_pair(thread, module);
		this->pending_modules++;
	}
	
}

void InstanceManager::runAll(bool autostart) {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "Attempting to run all modules.");
	std::list<std::string> * module_list = this->modulemanager.getModuleList();

	foreach(std::string module_name, *module_list) {
		this->run(module_name, autostart);
	}

}

void InstanceManager::tick() {
	using namespace firestarter::protocol::module;

	LOG_DEBUG(logger, "Called tick().");

	RunlevelResponse response;
	while (this->socket.receive(response)) {
		LOG_DEBUG(logger, "Received status changed message to " << response.runlevel());
		this->pending_modules--;
	}

	if (this->pending_modules > 0) {
		RunlevelRequest request;
		request.set_type(UPDATE);
		request.set_runlevel(INIT);
		request.set_immediate(true);
		this->socket.send(request);
	}

}
