#ifndef __INSTANCEMANAGER_H
#define __INSTANCEMANAGER_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.hpp"
#include "modulemanager.hpp"
#include "zmq/zmqsocket.hpp"
#include "module.hpp"

#include <list>
#include <boost/thread.hpp>

namespace firestarter {
	namespace InstanceManager {

typedef boost::unordered_map<std::string, firestarter::module::Module *> InstanceMap;
typedef boost::unordered_map<std::string, std::pair<boost::thread *, firestarter::module::RunnableModule *> > ThreadMap;

class InstanceManagerSocket {
	private:
	firestarter::sockets::ZMQPublisherSocket publisher;
	firestarter::sockets::ZMQResponseSocket responder;

	public:
	InstanceManagerSocket(zmq::context_t & context) :
			publisher(context, MODULE_ORDERS_SOCKET_URI), responder(context, MANAGER_SOCKET_URI) { };
	inline bool send(google::protobuf::Message & pb_message) { return this->publisher.send(pb_message); };
	inline bool reply(google::protobuf::Message & pb_message) { return this->responder.send(pb_message); };
	inline bool receive(google::protobuf::Message & pb_message, bool blocking = false) { 
		if (this->responder.receive(pb_message, blocking))
			return this->ack();
		return false;
	 };
	inline bool ack() { return this->responder.send(); };
};

/** \brief Manages the running instances of modules
  *
  */
class InstanceManager {
	private:
	firestarter::ModuleManager::ModuleManager & modulemanager;
	InstanceMap instances;
	ThreadMap threads;
	zmq::context_t & context;
	InstanceManagerSocket socket;
	bool running;
	int pending_modules;

	public:
	InstanceManager(firestarter::ModuleManager::ModuleManager & modulemanager, zmq::context_t & context) 
			throw(std::invalid_argument); 
	void run(const std::string & name, bool autostart = false) 
			throw(firestarter::exception::ModuleNotFoundException);
	void runAll(bool autostart = false);
	void stop(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	void stopAll();
	inline bool isRunning() { return this->running; };

};

/* Closing the namespace */
	}
}

#endif
