#include "instancemanager.h"

#ifndef logger
namespace firestarter { namespace InstanceManager {
	DECLARE_LOG(logger, "firestarter.InstanceManager");
} }
#endif

using namespace firestarter::InstanceManager;

InstanceManager::InstanceManager(firestarter::ModuleManager::ModuleManager & modulemanager, zmq::context_t & context) throw(std::invalid_argument)
							: modulemanager(modulemanager), context(context), modules(context, ZMQ_REQ),
							  running(false) {

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

	this->running = true;
	this->instances[name] = module_info->instantiate(this->context);

	if (module_info->shouldRunStandAlone()) {
		LOG_ERROR(logger, "Module `" << name << "' wants to run in its own process, but this hasn't been implemented yet.");
		/// \todo Throw an exception instead of returning
		/// \todo Implement process-spawning
		return;
	}

	if (module_info->shouldRunThreaded()) {
		using namespace firestarter::module;

		LOG_INFO(logger, "Spawning thread for module `" << name << "'.");
		RunnableModule * module = reinterpret_cast<RunnableModule *>(this->instances[name]);
		boost::thread * thread = new boost::thread(&RunnableModule::run, module);
		this->threads[name] = std::make_pair(thread, module);
	}
	
}

void InstanceManager::runAll(bool autostart) {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "Attempting to run all modules.");
	std::list<std::string> * module_list = this->modulemanager.getModuleList();

	foreach(std::string module_name, *module_list) {
		this->run(module_name, autostart);
	}

	RunlevelChangeRequest request;
	request.set_runlevel(INIT);
	request.set_immediate(true);
	this->send(request, this->modules);
}

void InstanceManager::send(google::protobuf::Message & pb_message, zmq::socket_t & socket) {
	LOG_INFO(logger, "Sending message (" << pb_message.GetTypeName() << ") on socket (" << &socket << ").");
	std::string pb_serialised;
	LOG_DEBUG(logger, "Serialising message.");
	pb_message.SerializeToString(&pb_serialised);
	zmq::message_t message(pb_serialised.size());
	LOG_DEBUG(logger, "Copying message data to socket.");
	memcpy((void *) message.data(), pb_serialised.c_str(), pb_serialised.size());
	LOG_DEBUG(logger, "Sending message");
	/** \todo Test return value to see if sending succeeded */
	socket.send(message);
}

int InstanceManager::pollIn(zmq::socket_t & socket) {
	zmq_pollitem_t item;
	item.socket = (void *)(&this->modules);
	item.fd = 0;
	item.events = ZMQ_POLLIN;

	return zmq::poll(&item, 1, 1);
}

void InstanceManager::tick() {
	using namespace firestarter::protocol::module;

	LOG_DEBUG(logger, "Called tick().");

	zmq::message_t response;
	if (this->modules.recv(&response, 1)) {
		LOG_DEBUG(logger, "Received a response!");
		RunlevelChangeResponse pb_response;
		pb_response.ParseFromString(static_cast<const char *>(response.data()));
		LOG_DEBUG(logger, "Received status changed message to " << pb_response.runlevel());
	}

}
