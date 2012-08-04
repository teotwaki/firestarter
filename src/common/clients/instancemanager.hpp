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

#ifndef FIRESTARTER_CLIENT_INSTANCEMANAGER_HPP
#define FIRESTARTER_CLIENT_INSTANCEMANAGER_HPP

#include "zmq/zmqsocket.hpp"
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
