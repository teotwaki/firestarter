
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

	public:
	virtual void setup() = 0; /**< pure virtual */
};

class RunnableModule : public Module {
	protected:
	bool running;
	zmq::context_t * context;
	zmq::socket_t * manager;

	inline void createManagementSocket() { 
		this->manager = new zmq::socket_t(*(this->context), ZMQ_REP); 
		this->manager->connect(MANAGER_SOCKET_URI); 
	};
	virtual void send(google::protobuf::Message & pb_message, zmq::socket_t * socket);

	public:
	RunnableModule();
	void setContext(zmq::context_t * context) { this->context = context; };
	virtual void shutdown() { LOG_WARN(logger, "shutdown() not implemented in RunnableModule (this = " << this << ")!"); };
	virtual void restart() { LOG_WARN(logger, "restart() not implemented in RunnableModule (this = " << this << ")!"); };
	
};

typedef Module * create_module();
typedef void destroy_module(Module *);
typedef int module_version();

/* Close namespaces */
	}
}

#endif
