
#ifndef __MODULE_H
#define __MODULE_H

#include <string>
#include <boost/tr1/unordered_map.hpp>
#include <cstdlib>
#include <ctime>

#include "helper.h"
#include "clients/instancemanager.h"
#include "protobuf/module.pb.h"
#include "zmq/zmqhelper.h"

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
	
};

typedef Module * create_module(zmq::context_t & context);
typedef void destroy_module(Module *);
typedef int module_version();

/* Close namespaces */
	}
}

#endif
