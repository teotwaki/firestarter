
#ifndef __ZMQSOCKET_H
#define __ZMQSOCKET_H

#include "zmq/zmqhelper.hpp"
#include "protobuf/module.pb.h"
#include "log.hpp"

#include <list>
#include <string>

/* Most of the documentation in this file is rephrased/amended information coming from the ZMQ documentation.
 * It can be found at http://api.zeromq.org/2-2:_start
 */

namespace firestarter {
	namespace sockets {

/** \brief Base class for all sockets
  *
  * This is the base class for all the ZMQ-based sockets. Currently two classes inherit from this class: 
  * ZMQReceivingSocket and ZMQSendingSocket. The idea is to abstract the features provided by ZMQ and more specifically
  * the zmqcpp binding as it lacks in basic polymorphism features.
  *
  * This class also provides the pollable() method, which returns a void * that can be used with the zmq_polling 
  * interface.
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

	/** \brief Destroy the object and the underlying socket if it exists */
	~ZMQSocket() {
		if (this->socket != NULL)
			delete this->socket;
	};

	public:
	/** \brief Return a pointer that can be used with zmq_poll() */
	inline void * pollable() { return static_cast<void *>(this->socket); };
};

/** \brief Base class for all sockets types that can receive data
  *
  * This class specialises the ZMQSocket base class by adding the receive() and connect() methods. It is inherited by
  * the ZMQSubscriberSocket, ZMQRequestSocket and ZMQResponseSocket classes.
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
	  * \param blocking When set to true, the function will be blocking until a message is received on the socket, or
	  * an interrupt signal. The default is false (non-blocking call).
	  */
	bool receive(/** [in] */ bool blocking = false);

	/** \brief Receive a single message and deserialise it into a protobuf message
	  *
	  * Receive a message on the socket, deserialise it (using pb_message.ParseFromArray()) and store the results in
	  * the pb_message passed as parameter.
	  *
	  * \return True when a message was received, false otherwise.
	  *
	  * \param pb_message A reference to a pb_message object. The object must be able to deserialise the message
	  * waiting on the wire, otherwise the contents will be lost.
	  * \param blocking When set to true, the function will be blocking until a message is received on the socket, or
	  * an interrupt signal. The default is false (non-blocking call).
	  */
	bool receive(/** [out] */ google::protobuf::Message & pb_message, /** [in] */ bool blocking = false);

	/** \brief Connect the socket the a remote endpoint
	  *
	  * Connect the socket to a remote endpoint, as specified by the ZMQ API. This method should be called by most
	  * constructors of classes inheriting this one.
	  *
	  * \param uri A reference to a string containing the enpoint to which the socket should connect.
	  */
	inline void connect(/** [in] */ std::string const & uri) { 
		if (not uri.empty())
			this->socket->connect(uri.c_str()); 
		/** \todo Else throw exception */
	};

	/** \brief Connect to a list a remote endpoints
	  *
	  * This is basically a wrapper around the regular connect() method, enabling the user to connect to a number of
	  * endpoints in a single call.
	  *
	  * \param uris A reference to a list of strings containing the endpoints to which the socket should connect.
	  */
	inline void connect(/** [in] */ std::list<std::string> const & uris) {
		for (std::string const & uri : uris) {
			this->connect(uri);
		}
	};
};

/** \brief Base class for all sockets types that can send data
  *
  * This class specialises the ZMQSocket base class by adding the send() and bind() methods. It is inherited by the
  * ZMQPublisherSocket, ZMQRequestSocket and ZMQResponseSocket classes.
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
	  * This method can be useful for sending an acknowledgement message. For example, when a Rep/Req connection is
	  * established, in order for the client to send another message the server needs to send a message, even an empty
	  * one.
	  *
	  * \return True when the message was sent, false otherwise.
	  */
	bool send();

	/** \brief Send a protobuf message
	  *
	  * This method takes a reference to a pb_message and tries to send it on the socket.
	  *
	  * \return True when the message was sent, false otherwise.
	  *
	  * \param send_more When set to true, the function indicates to the ZMQ layer that the current message is a
	  * multipart one. A multipart message enables the segmentation of messages, that will all be sent and delivered at
	  * once in an atomical manner.
	  */
	bool send(/** [in] */ google::protobuf::Message const & pb_message, /** [in] */ bool send_more = false);

	/** \brief Bind the socket to a local endpoint for incoming connections
	  *
	  * This method binds the socket to a specific endpoint for other sockets to connect to. If the string has a
	  * zero-length, no binding will be done, and the method will silently fail.
	  *
	  * \param uri A reference to a string containing the endpoint to which the socket should bind.
	  */
	inline void bind(/** [in] */ std::string const & uri) { 
		if (not uri.empty())
			this->socket->bind(uri.c_str()); 
		/** \todo Else throw exception */
	};

	/** \brief Bind the socket to a list of local endpoint for incoming connections
	  *
	  * This is basically a wrapper around the regular bind() method, enabling the user to bind a number of endpoints
	  * in a single call. If any of the strings are zero-length, it will simply be ignored. However, the other items in
	  * the list will still be processed.
	  *
	  * \param uris A reference to a list of strings containing the endpoints to which the socket should bind.
	  */
	inline void bind(/** [in] */ std::list<std::string> const & uris) {
		for (std::string const & uri : uris) {
			this->bind(uri);
		}
	};
};

/** \brief A publisher (fan-out) send-only socket
  *
  * A publisher socket provides a one-to-many (fan-out) sending socket. It is capable of sending data to multiple
  * subscribers, but however is unable to receive any data, which is why it only implements the ZMQSendingSocket class.
  * Subscribers should use the ZMQSubscriberSocket class.
  *
  * Messages are sent out to all connected subscribers simultaneously (with the obvious caveats linked to network
  * delays and lost packets). If a specific subscriber is unable to process all data and the connection to that
  * specific subscriber reaches the ZMQ high water mark, all messages will be dropped until the exceptional state ends.
  *
  * According to the ZMQ specification, send() is guaranteed to never block for this kind of socket.
  *
  * \see ZMQSendingSocket
  * \see ZMQSubscriberSocket
  */
class ZMQPublisherSocket : public ZMQSendingSocket {
	public:
	/** \brief Constructor which initialises the underlying socket
	  *
	  * The constructor initialises the underlying ZMQ socket by using the provided ZMQ context. If a uri is provided,
	  * the socket will bind to it. The socket is usable immediately (if a uri has been provided).
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uri Optional reference to a string on which the socket should bind.
	  **/
	ZMQPublisherSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::string const & uri = std::string()) { 
		this->socket = new zmq::socket_t(context, ZMQ_PUB); 
		this->bind(uri); 
	};

	/** \brief Constructor which initialises the underlying socket and binds to a list of uris
	  *
	  * This constructor provides the same functionalities as the other one, with the exception that it requires a list
	  * of uris to which the socket should bind to.
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uris A reference to a list of strings on which the socket should bind.
	  */
	ZMQPublisherSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::list<std::string> const & uris) {
		this->socket = new zmq::socket_t(context, ZMQ_PUB);
		this->bind(uris);
	};
};

/** \brief A subscriber (fan-in) receive-only socket
  *
  * A subscriber sockets provides the capability to subscribe to one or multiple publisher sockets. It is capable of
  * receiving data from multiple publishers, but is however unable to send any data, which is why it only implements
  * the ZMQReceivingSocket class. Publishers should use the ZMQPublisherSocket class.
  *
  * Messages are received from all connected publishers by using a fair-queued routing strategy. If the subscriber is
  * unable to process all the messages coming from publishers, the publishers will eventually reach their high water
  * mark and begin dropping packets.
  *
  * \see ZMQReceivingSocket
  * \see ZMQPublisherSocket
  */
class ZMQSubscriberSocket : public ZMQReceivingSocket {
	public:
	/** \brief Constructor which initialises the underlying socket
	  *
	  * The constructor initialises the underlying ZMQ socket by using the provided ZMQ context. If a uri is provided,
	  * the socket will connect to it. The socket is usable immediately (if a uri has been provided).
	  *
	  * By default, the socket will subscribe to any kind of message, unless the third argument is set to false.
	  *
	  * \see connect()
	  * \see subscribe()
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uri Optional reference to a string on which the socket should connect to.
	  * \param subscribe If set to true, the socket will automatically subscribe to any kind of messages. If set to
	  * false, no subscription will be done.
	  */
	ZMQSubscriberSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::string const & uri = std::string(), 
						/** [in] */ bool subscribe = true) { 
		this->socket = new zmq::socket_t(context, ZMQ_SUB); 
		this->connect(uri);
		if (subscribe)
			this->subscribe();
	};

	/** \brief Constructor which initialises the underlying socket and connects to a list of uris
	  *
	  * This constructor provides the same functionalities as the other one, with the exception that it requires a list
	  * of uris to which the socket should connect to.
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uris A reference to a list of strings to which the socket should connect to.
	  * \param subscribe If set to true, the socket will automatically subscribe to any kind of messages. If set to
	  * false, no subscription will be done.
	  */
	ZMQSubscriberSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::list<std::string> const & uris,
						/** [in] */ bool subscribe = true) {
		this->socket = new zmq::socket_t(context, ZMQ_SUB);
		this->connect(uris);
		if (subscribe)
			this->subscribe();
	};

	/** \brief Subscribe to a specific type of message
	  *
	  * Establish a new message filter that will be applied to incoming data. The content of the filter is binary data.
	  * A zero-length string will subscribe to all data. If the same filter is applied multiple times, they will all be
	  * treated as separate filters.
	  *
	  * \param filter A reference to a string containing the filter data that should be added.
	  */ 
	inline void subscribe(/** [in] */ std::string const & filter = std::string()) { 
		this->socket->setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.size()); 
	};

	/** \brief Unsubscribe to a specific type of message
	  *
	  * Remove an existing message filter. If there are multiple equivalent filters applied to the socket, only one
	  * instance of said filter will be removed.
	  *
	  * \param filter A reference to a string containing the filter data that should be removed
	  */
	inline void unsubscribe(/** [in] */ std::string const & filter = std::string()) {
		this->socket->setsockopt(ZMQ_UNSUBSCRIBE, filter.c_str(), filter.size());
	};
};

/** \brief An alternating request-reply (send/receive) socket
  *
  * Typically, a ZMQRequestSocket connects to a service and alternatingly sends requests and receives replies. Once a
  * request has been sent, it is illegal to send another request. Before being able to send another request, receive()
  * has to be called on the socket. Service providers should use the ZMQResponseSocket.
  * 
  * The response doesn't necessarily have to contain an actual message; an empty message that acquits the send/receive
  * exchange is sufficient.
  *
  * Messages are sent to all services in a round-robin fashion. This means that if there are two services connected to
  * on the socket, and 3 messages are sent, message 1 will go to service 1, message 2 to service 2, and message 3 to
  * service 1. If the socket is connected to zero services or if the high water mark is reached, then the send() call
  * will be blocking until a service becomes available, or the exceptional state ends.
  *
  * Please note that calling bind() on this socket is nonsensical at the moment. Future versions of this API will
  * probably refactor the classes to remove bind() from the member methods.
  *
  * \see ZMQReceivingSocket
  * \see ZMQSendingSocket
  * \see ZMQResponseSocket
  */
class ZMQRequestSocket : public ZMQSendingSocket, public ZMQReceivingSocket {
	public:
	/** \brief Initialise the underlying socket and optionally connects to a service.
	  *
	  * The constructor initialises the underlying ZMQ socket by using the provided ZMQ context. If a uri is provided,
	  * the socket will connect to it. The socket is usable immediately (if a uri has been provided).
	  *
	  * \see connect()
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uri Optional reference to a string to which the socket should connect.
	  */
	ZMQRequestSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::string const & uri = std::string()) { 
		this->socket = new zmq::socket_t(context, ZMQ_REQ); 
		this->connect(uri); 
	};

	/** \brief Constructor which initialises the underlying socket and connects to a list of uris
	  *
	  * This constructor provides the same functionalities as the other one, with the exception that it requires a list
	  * of uris to which the socket should connect to.
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uris A reference to a list of strings to which the socket should connect.
	  */
	ZMQRequestSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::list<std::string> const & uris) {
		this->socket = new zmq::socket_t(context, ZMQ_REQ);
		this->connect(uris);
	};
};

/** \brief An alternating reply-request (receive/send) socket
  *
  * Typically, a ZMQResponseSocket provides a service and alternatingly receives requests and sends replies. Once a
  * request has been received, it is illegal to receive another request. Before being able to receive another request,
  * send() has to be called on the socket. Clients connecting to the service should use the ZMQRequestSocket class.
  * 
  * The response doesn't necessarily have to contain an actual message; an empty message that acquits the send/receive
  * exchange is sufficient. Calling send() with no arguments, for example provides this "acknowledgement" feature.
  *
  * Messages are fair-queued from all incoming clients, and responses are ensured to be routed to the last client who
  * sent a request. If the original client who sent the request is no longer available, the socket will silently
  * discard the response. If the sockets hits the high water mark while sending data to a client, all responses will be
  * silently discarded.
  *
  * Please note that calling connect() on this socket is nonsensical at the moment. Future versions of this API will
  * probably refactor the classes to remove connect() from the member methods.
  *
  * \see ZMQReceivingSocket
  * \see ZMQSendingSocket
  * \see ZMQRequestSocket
  */
class ZMQResponseSocket : public ZMQSendingSocket, public ZMQReceivingSocket {
	public:
	/** \brief Initialise the underlying socket and optionally connects to a service.
	  *
	  * The constructor initialises the underlying ZMQ socket by using the provided ZMQ context. If a uri is provided,
	  * the socket will bind to it. The socket is usable immediately (if a uri has been provided).
	  *
	  * \see bind()
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uri Optional reference to a string on which the socket should bind.
	  */
	ZMQResponseSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::string const & uri = std::string()) { 
		this->socket = new zmq::socket_t(context, ZMQ_REP); 
		this->bind(uri); 
	};

	/** \brief Constructor which initialises the underlying socket and connects to a list of uris
	  *
	  * This constructor provides the same functionalities as the other one, with the exception that it requires a list
	  * of uris to which the socket should bind to.
	  *
	  * \param context A reference to the ZMQ context in which the socket should be created.
	  * \param uris A reference to a list of strings to which the socket should bind.
	  */
	ZMQResponseSocket(/** [in] */ zmq::context_t & context, /** [in] */ std::list<std::string> const & uris) {
		this->socket = new zmq::socket_t(context, ZMQ_REP);
		this->bind(uris);
	};
};

/* Close namespaces */
	}
}

#endif
