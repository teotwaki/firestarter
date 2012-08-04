
#ifndef FIRESTARTER_MODULE_HPP
#define FIRESTARTER_MODULE_HPP

#include <string>
#include <cstdlib>
#include <ctime>

#include "helper.hpp"
#include "clients/instancemanager.hpp"
#include "protobuf/module.pb.h"
#include "zmq/zmqhelper.hpp"

namespace firestarter {
	namespace module {

class Module {
	public:
	virtual void setup() = 0; /**< pure virtual */
};

class RunnableModule : public Module {
	protected:
	bool running;
	firestarter::InstanceManager::InstanceManagerClientSocket manager_socket;
	firestarter::protocol::module::RunLevel runlevel;

	RunnableModule(zmq::context_t & context) : running(false), manager_socket(context) , runlevel(firestarter::protocol::module::NONE) { };

	public:
	virtual void run() = 0; /**< pure virtual */
	virtual void shutdown();
	virtual void restart();
	virtual void _initialiser();
	
};

typedef Module * create_module(zmq::context_t & context);
typedef void destroy_module(Module *);
typedef int module_version();

/* Close namespaces */
	}
}

#endif
