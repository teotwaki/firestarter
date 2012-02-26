
#ifndef __MODULE_H
#define __MODULE_H

#include <string>
#include <boost/tr1/unordered_map.hpp>
#include <cstdlib>
#include <ctime>

#include "helper.h"
#include "zmqhelper.h"
#include "protobuf/module.pb.h"

namespace firestarter {
	namespace module {

class Module {
	protected:
	zmq::context_t & context;

	Module(zmq::context_t & context) : context(context) { };

	public:
	virtual void setup() = 0; /**< pure virtual */
};

class RunnableModule : public Module {
	private:
	void createManagementSocket();

	protected:
	bool running;
	zmq::socket_t * orders;
	zmq::socket_t * manager;
	firestarter::protocol::module::RunLevel runlevel;

	RunnableModule(zmq::context_t & context) : Module(context), running(false), orders(NULL), manager(NULL), runlevel(firestarter::protocol::module::NONE) { this->createManagementSocket(); };
	virtual void send(google::protobuf::Message & pb_message, zmq::socket_t * socket);

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
