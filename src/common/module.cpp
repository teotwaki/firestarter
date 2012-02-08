#include "module.h"

using namespace firestarter::module;

void RunnableModule::send(google::protobuf::Message & pb_message, zmq::socket_t * socket) {
	std::string pb_serialised;
	pb_message.SerializeToString(&pb_serialised);
	zmq::message_t message(pb_serialised.size());
	memcpy(static_cast<void *>(message.data()), pb_serialised.c_str(), pb_serialised.size());
	socket->send(message);
}	
