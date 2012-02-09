
#ifndef __MODULE_H
#define __MODULE_H

#include <string>
#include <boost/tr1/unordered_map.hpp>

#include "helper.h"
#include "zmqhelper.h"
#include "protobuf/module.pb.h"

DECLARE_EXTERN_LOG(logger);

namespace firestarter {
	namespace module {


class Module {
	protected:
	zmq::context_t * context;

	Module(zmq::context_t * context) : context(context) { };

	public:
	virtual void setup() = 0; /**< pure virtual */
	void setContext(zmq::context_t * context) { this->context = context; };
};

class RunnableModule : public Module {
	protected:
	bool running;
	zmq::socket_t * manager;

	RunnableModule(zmq::context_t * context) : Module(context), running(false), manager(NULL) { };
	void createManagementSocket();
	virtual void send(google::protobuf::Message & pb_message, zmq::socket_t * socket);

	public:
	virtual void run() = 0; /**< pure virtual */
	virtual void shutdown() { LOG_WARN(logger, "shutdown() not implemented in RunnableModule (this = " << this << ")!"); };
	virtual void restart() { LOG_WARN(logger, "restart() not implemented in RunnableModule (this = " << this << ")!"); };
	
};

typedef Module * create_module(zmq::context_t * context);
typedef void destroy_module(Module *);
typedef int module_version();

/* Close namespaces */
	}
}

#endif
