#include "zmq/zmqsocket.h"

namespace firestarter { namespace sockets {
	DECLARE_LOG(logger, "firestarter.sockets");
} }

using namespace firestarter::sockets;

bool ZMQSendingSocket::send() {
	LOG_INFO(logger, "Sending empty message on socket (" << &(this->socket) << ").");
	zmq::message_t message(0);
	return socket->send(message, 0) == 0;
}

bool ZMQSendingSocket::send(google::protobuf::Message & pb_message, bool send_more) {
	LOG_INFO(logger, "Sending message (" << pb_message.GetTypeName() << ") on socket (" << &(this->socket) << ").");

	LOG_DEBUG(logger, "Serialising message.");
	std::string pb_serialised;
	pb_message.SerializeToString(&pb_serialised);

	LOG_DEBUG(logger, "Copying message data to socket (" << &(this->socket) << ").");
	zmq::message_t message(pb_serialised.size());
	memcpy(static_cast<void *>(message.data()), pb_serialised.c_str(), pb_serialised.size());

	int flags = 0;
	if (send_more) {
		LOG_DEBUG(logger, "Added ZMQ_SNDMORE flag to socket.");
		flags = ZMQ_SNDMORE;
	}

	else {
		LOG_DEBUG(logger, "No flags to add.");
	}

	LOG_DEBUG(logger, "Sending message.");
	return this->socket->send(message, flags) == 0;
}

bool ZMQReceivingSocket::receive(bool blocking) {
	LOG_INFO(logger, "Listening for an empty message on socket (" << &(this->socket) << ").");
	zmq::message_t message;

	int flags = 0;
	if (not blocking) {
		LOG_DEBUG(logger, "Added ZMQ_DONTWAIT flag to socket.");
		flags = ZMQ_DONTWAIT;
	}

	else {
		LOG_DEBUG(logger, "No flags to add.");
	}

	return this->socket->recv(&message, flags) == 0;
}

bool ZMQReceivingSocket::receive(google::protobuf::Message & pb_message, bool blocking) {
	LOG_INFO(logger, "Listening for a " << pb_message.GetTypeName() << " on socket (" << &(this->socket) << ").");
	zmq::message_t message;

	int flags = 0;
	if (not blocking) {
		LOG_DEBUG(logger, "Added ZMQ_DONTWAIT flag to socket.");
		flags = ZMQ_DONTWAIT;
	}

	else {
		LOG_DEBUG(logger, "No flags to add.");
	}

	if (this->socket->recv(&message, flags))
		if (pb_message.ParseFromArray(message.data(), message.size()))
			return pb_message.IsInitialized();

	return false;
}

