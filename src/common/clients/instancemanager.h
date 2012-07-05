#ifndef __CLIENT_INSTANCEMANAGER_H
#define __CLIENT_INSTANCEMANAGER_H

#include "zmq/zmqsocket.h"
#include "protobuf/module.pb.h"

namespace firestarter {
	namespace InstanceManager {

class InstanceManagerClientSocket {
	private:
	firestarter::sockets::ZMQSubscriberSocket subscriber;
	firestarter::sockets::ZMQRequestSocket requester;

	public:
	InstanceManagerClientSocket(zmq::context_t & context) :
									subscriber(context, MODULE_ORDERS_SOCKET_URI), requester(context, MANAGER_SOCKET_URI) { };
	inline bool send(google::protobuf::Message & pb_message) { 
		if (this->requester.send(pb_message))
			return this->receive_ack();
		return false;
	};
	inline bool receive(google::protobuf::Message & pb_message, bool blocking = false) { return this->subscriber.receive(pb_message, blocking); };
	inline bool receive_ack() { return this->requester.receive(true); };
};

/* Closing namespaces */
	}
}

#endif
