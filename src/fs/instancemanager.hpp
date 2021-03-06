/*
 * Copyright (C) 2012  Sebastian Lauwers <sebastian.lauwers@gmail.com>
 *
 * This file is part of Firestarter.
 *
 * Firestarter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firestarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRESTARTER_INSTANCEMANAGER_HPP
#define FIRESTARTER_INSTANCEMANAGER_HPP

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
