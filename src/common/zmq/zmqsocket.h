
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

/** \brief Base class for all sockets
  *
  * This is the base class for all the ZMQ-based sockets. Currently two classes inherit
  * from this class: ZMQReceivingSocket and ZMQSendingSocket. The idea is to abstract
  * the features provided by ZMQ and more specifically the zmqcpp binding as it lacks
  * in basic polymorphism features.
  *
  * This class also provides the pollable() method, which returns a void * that can be
  * used with the zmq_polling interface.
  *
  * \see ZMQReceivingSocket
  * \see ZMQSendingSocket
  * \see ZMQ CPP bindings
  *
  */
class ZMQSocket {
	protected:
	/** \brief Pointer to a zmq socket */
	zmq::socket_t * socket;

	/** \brief The constructor simply initialises the pointer to NULL */
	ZMQSocket() : socket(NULL) { };

	public:
	/** \brief Return a pointer that can be used with zmq_poll() */
	inline void * pollable() { return static_cast<void *>(this->socket); };
};

/** \brief Base class for all sockets types that can receive data
  *
  * This class specialises the ZMQSocket base class by adding the receive() and connect()
  * methods. It is inherited by the ZMQSubscriberSocket, ZMQRequestSocket and 
  * ZMQResponseSocket classes.
  *
  * \see ZMQSubscriberSocket
  * \see ZMQRequestSocket
  * \see ZMQResponseSocket
  *
  */
class ZMQReceivingSocket : virtual public ZMQSocket {
	public:
	/** \brief Receive a single message and discard the contents
	  *
	  * This method can be useful for receiving acknowledgement messages.
	  *
	  * \return True when a message was received, false otherwise.
	  *
	  * \param blocking When set to true, the function will be blocking until a message is
	  * received on the socket, or an interrupt signal. The default is false (non-blocking
	  * call).
	  */
	bool receive(/** [in] */ bool blocking = false);
	/** \brief Receive a single message and deserialise it into a protobuf message
	  *
	  * Receive a message on the socket, deserialise it (using pb_message.ParseFromArray())
	  * and store the results in the pb_message passed as parameter.
	  *
	  * \return True when a message was received, false otherwise.
	  *
	  * \param pb_message A reference to a pb_message object. The object must be able to
	  * deserialise the message waiting on the wire, otherwise the contents will be lost.
	  * \param blocking When set to true, the function will be blocking until a message is
	  * received on the socket, or an interrupt signal. The default is false (non-blocking
	  * call).
	  */
	bool receive(/** [out] */ google::protobuf::Message & pb_message, /** [in] */ bool blocking = false);
	/** \brief Connect the socket the a remote endpoint
	  *
	  * Connect the socket to a remote endpoint, as specified by the ZMQ API. This method
	  * should be called by most constructors of classes inheriting this one.
	  *
	  * \param uri A reference to a string containing the enpoint to which the socket
	  * should connect.
	  */
	inline void connect(/** [in] */ std::string & uri) { 
		if (not uri.empty())
			this->socket->connect(uri.c_str()); 
		/** \todo Else throw exception */
	};
	/** \brief Connect to a list a remote endpoints
	  *
	  * This is basically a wrapper around the regular connect() method, enabling the user
	  * to connect to a number of endpoints in a single call.
	  *
	  * \param uris A reference to a list of strings containing the endpoints to which the
	  * socket should connect.
	  */
	inline void connect(/** [in] */ std::list<std::string> & uris) {
		foreach (std::string uri, uris) {
			this->connect(uri);
		}
	};
};

/** \brief Base class for all sockets types that can send data
  *
  * This class specialises the ZMQSocket base class by adding the send() and bind()
  * methods. It is inherited by the ZMQPublisherSocket, ZMQRequestSocket and 
  * ZMQResponseSocket classes.
  *
  * \see ZMQPublisherSocket
  * \see ZMQRequestSocket
  * \see ZMQResponseSocket
  *
  */
class ZMQSendingSocket : virtual public ZMQSocket {
	public:
	/** \brief Send an empty message
	  *
	  * This method can be useful for sending an acknowledgement message. For example,
	  * when a Rep/Req connection is established, in order for the client to send another
	  * message the server needs to send a message, even an empty one.
	  *
	  * \return True when the message was sent, false otherwise.
	  *
	  * \param send_more When set to true, the function indicates to the ZMQ layer that
	  * the current message is a multipart one. A multipart message enables the segmentation
	  * of messages, that will all be sent and delivered at once in an atomical manner.
	  */
	bool send();
	/** \brief Send a protobuf message
	  *
	  * This method takes a reference to a pb_message and tries to send it on the socket.
	  *
	  * \return True when the message was sent, false otherwise.
	  *
	  * \param send_more When set to true, the function indicates to the ZMQ layer that
	  * the current message is a multipart one. A multipart message enables the segmentation
	  * of messages, that will all be sent and delivered at once in an atomical manner.
	  */
	bool send(/** [in] */ google::protobuf::Message & pb_message, /** [in] */ bool send_more = false);
	/** \brief Bind the socket to a local endpoint for incoming connections
	  *
	  * This method binds the socket to a specific endpoint for other sockets to connect to.
	  *
	  * \param uri A reference to a string containing the endpoint to which the socket
	  * should bind.
	  */
	inline void bind(/** [in] */ std::string & uri) { 
		if (not uri.empty())
			this->socket->bind(uri.c_str()); 
		/** \todo Else throw exception */
	};
	/** \brief Bind the socket to a list of local endpoint for incoming connections
	  *
	  * This is basically a wrapper around the regular bind() method, enabling the user
	  * to bind a number of endpoints in a single call.
	  *
	  * \param uris A reference to a list of strings containing the endpoints to which the
	  * socket should bind.
	  */
	inline void bind(/** [in] */ std::list<std::string> & uris) {
		foreach (std::string uri, uris) {
			this->bind(uri);
		}
	};
};

class ZMQPublisherSocket : public ZMQSendingSocket {
	public:
	ZMQPublisherSocket(zmq::context_t & context, std::string & uri = "") { this->socket = new zmq::socket_t(context, ZMQ_PUB); this->bind(uri); };
};

class ZMQSubscriberSocket : public ZMQReceivingSocket {
	public:
	ZMQSubscriberSocket(zmq::context_t & context, std::string & uri = "") { 
		this->socket = new zmq::socket_t(context, ZMQ_SUB); 
		this->connect(uri); 
		this->subscribe();
	};
	inline void subscribe(std::string & filter = "") { 
		this->socket->setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.size()); 
	};
	inline void unsubscribe(std::string & filter = "") {
		this->socket->setsockopt(ZMQ_UNSUBSCRIBE, filter.c_str(), filter.size());
	};
};

class ZMQRequestSocket : public ZMQSendingSocket, public ZMQReceivingSocket {
	public:
	ZMQRequestSocket(zmq::context_t & context, std::string & uri = "") { this->socket = new zmq::socket_t(context, ZMQ_REQ); this->connect(uri); };
};

class ZMQResponseSocket : public ZMQSendingSocket, public ZMQReceivingSocket {
	public:
	ZMQResponseSocket(zmq::context_t & context, std::string & uri = "") { this->socket = new zmq::socket_t(context, ZMQ_REP); this->bind(uri); };
};

/* Close namespaces */
	}
}

#endif
