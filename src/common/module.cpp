#include "module.h"

namespace firestarter { namespace module {
	DECLARE_EXTERN_LOG(logger);
} }

using namespace firestarter::module;

void RunnableModule::shutdown() { 
	LOG_WARN(logger, "shutdown() not implemented in RunnableModule (this = " << this << ")!");
}

void RunnableModule::restart() { 
	LOG_WARN(logger, "restart() not implemented in RunnableModule (this = " << this << ")!");
};

void RunnableModule::_initialiser() {
	using namespace firestarter::protocol::module;

	LOG_DEBUG(logger, "Waiting for InstanceManager orders");

	while (this->running || this->runlevel == NONE) {
		RunlevelRequest order;

		if (this->manager_socket.receive(order, true) && order.type() == UPDATE) {
			LOG_DEBUG(logger, "Message received from manager.");
		}

		else {
			LOG_ERROR(logger, order.DebugString());
			LOG_ERROR(logger, order.InitializationErrorString());
			if (order.IsInitialized()) {
				LOG_ERROR(logger, "Weird, the message is initialised!");
			}
			LOG_WARN(logger, "Couldn't parse message received from manager. Shutting down.");
			return;
		}

		switch(order.runlevel()) {
			case INIT: {
				LOG_DEBUG(logger, "Calling setup() after receiving INIT message.");
				this->setup();
				LOG_DEBUG(logger, "Creating response message");
				RunlevelResponse response;
				response.set_runlevel(order.runlevel());
				response.set_result(SUCCESS);
				if (not this->manager_socket.send(response)) {
					LOG_ERROR(logger, "Response couldn't be sent to manager!");
					LOG_INFO(logger, "ZMQ error (" << errno << ") message: " << zmq_strerror(zmq_errno()));
					return;
				}
				break;
			}

			case RUNNING: {
				LOG_DEBUG(logger, "Received RUNNING message, sending reply first.");
				LOG_DEBUG(logger, "Creating response message");
				RunlevelResponse response;
				response.set_runlevel(order.runlevel());
				response.set_result(SUCCESS);
				if (not this->manager_socket.send(response)) {
					LOG_ERROR(logger, "Response couldn't be sent to manager!");
					return;
				}
				LOG_DEBUG(logger, "Message sent, calling run().");
				this->run();
				break;
			}

			default:
				LOG_WARN(logger, "Unexpected runlevel change request received. Shutting down.");
				return;
				break;
		}
	}
};
