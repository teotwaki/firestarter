
#ifndef __ZMQSOCKET_H
#define __ZMQSOCKET_H

#include "zmq/zmqhelper.h"
#include "protobuf/module.pb.h"
#include "log.h"
#include "foreach.h"

#include <list>
#include <string>

namespace firestarter {
	namespace sockets {

class ZMQSocket {
	protected:
	zmq::socket_t * socket;

	ZMQSocket() : socket(NULL) { };

	public:
	inline void * pollable() { return static_cast<void *>(&(this->socket)); };
};

class ZMQReceivingSocket : virtual public ZMQSocket {
	protected:
	ZMQReceivingSocket() { };

	public:
	bool receive(bool blocking = false);
	bool receive(google::protobuf::Message & pb_message, bool blocking = false);
	inline void connect(std::string uri) { 
		if (not uri.empty())
			this->socket->connect(uri.c_str()); 
		/** \todo Else throw exception */
	};
	inline void connect(std::list<std::string> uris) {
		foreach (std::string uri, uris) {
			this->connect(uri);
		}
	};
};

class ZMQSendingSocket : virtual public ZMQSocket {
	protected:
	ZMQSendingSocket() { };

	public:
	bool send();
	bool send(google::protobuf::Message & pb_message, bool send_more = false);
	inline void bind(std::string uri) { 
		if (not uri.empty())
			this->socket->bind(uri.c_str()); 
		/** \todo Else throw exception */
	};
	inline void bind(std::list<std::string> uris) {
		foreach (std::string uri, uris) {
			this->bind(uri);
		}
	};
};

class ZMQPublisherSocket : public ZMQSendingSocket {
	public:
	ZMQPublisherSocket(zmq::context_t & context, std::string uri = "") { this->socket = new zmq::socket_t(context, ZMQ_PUB); this->bind(uri); };
};

class ZMQSubscriberSocket : public ZMQReceivingSocket {
	public:
	ZMQSubscriberSocket(zmq::context_t & context, std::string uri = "") { 
		this->socket = new zmq::socket_t(context, ZMQ_SUB); 
		this->connect(uri); 
		this->subscribe();
	};
	inline void subscribe(std::string filter = "") { 
		this->socket->setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.size()); 
	};
	inline void unsubscribe(std::string filter = "") {
		this->socket->setsockopt(ZMQ_UNSUBSCRIBE, filter.c_str(), filter.size());
	};
};

class ZMQRequestSocket : public ZMQSendingSocket, public ZMQReceivingSocket {
	public:
	ZMQRequestSocket(zmq::context_t & context, std::string uri = "") { this->socket = new zmq::socket_t(context, ZMQ_REQ); this->connect(uri); };
};

class ZMQResponseSocket : public ZMQSendingSocket, public ZMQReceivingSocket {
	public:
	ZMQResponseSocket(zmq::context_t & context, std::string uri = "") { this->socket = new zmq::socket_t(context, ZMQ_REP); this->bind(uri); };
};

/* Close namespaces */
	}
}

#endif
