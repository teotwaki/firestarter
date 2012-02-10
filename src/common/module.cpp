#include "module.h"

namespace firestarter { namespace module {
	DECLARE_EXTERN_LOG(logger);
} }

using namespace firestarter::module;

void RunnableModule::createManagementSocket() {
	LOG_INFO(logger, "Initialising socket to manager.");
	
	LOG_DEBUG(logger, "Creating new socket.");
	this->manager = new zmq::socket_t(this->context, ZMQ_REP); 
	LOG_DEBUG(logger, "Connecting to manager endpoint (" << MANAGER_SOCKET_URI << ").");
	this->manager->connect(MANAGER_SOCKET_URI); 
}

void RunnableModule::send(google::protobuf::Message & pb_message, zmq::socket_t * socket) {
	LOG_INFO(logger, "Sending message (" << pb_message.GetTypeName() << ") on socket (" << socket << ").");
	std::string pb_serialised;
	LOG_DEBUG(logger, "Serialising message.");
	pb_message.SerializeToString(&pb_serialised);
	zmq::message_t message(pb_serialised.size());
	LOG_DEBUG(logger, "Copying message data to socket.");
	memcpy(static_cast<void *>(message.data()), pb_serialised.c_str(), pb_serialised.size());
	LOG_DEBUG(logger, "Sending message");
	/** \todo Test return value to see if sending succeeded */
	socket->send(message);
}	

void RunnableModule::shutdown() { 
	LOG_WARN(logger, "shutdown() not implemented in RunnableModule (this = " << this << ")!");
}

void RunnableModule::restart() { 
	LOG_WARN(logger, "restart() not implemented in RunnableModule (this = " << this << ")!");
};
