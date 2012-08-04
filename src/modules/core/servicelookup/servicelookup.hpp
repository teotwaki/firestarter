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

#ifndef FIRESTARTER_SERVICELOOKUP_HPP
#define FIRESTARTER_SERVICELOOKUP_HPP

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.hpp"
#include "instancemanager.hpp"
#include "zmq/zmqsocket.hpp"

#include <list>
#include <boost/thread.hpp>

namespace firestarter {
	namespace module {
		namespace core {
			namespace ServiceLookup {

	typedef boost::unordered_map<std::string, std::string> ServiceMap;

	class ServiceLookupSocket {
		private:
		firestarter::sockets::ZMQResponseSocket responder;
	
		public:
		ServiceLookupSocket(zmq::context_t, & context) : responder(context, SERVICE_LOOKUP_URI) { };
		inline bool reply(google::protobuf::Message & pb_message) { return this->responder.send(pb_message); };
		inline bool receive(google::protobuf::Message & pb_message) {
			if (this->responder.receive(pb_message))
				return true;
			return false;
		};
	};
	
	class ServiceLookup : public RunnableModule {
		private:
		firestarter::InstanceManager::InstanceManager & instance_manager;
		ServiceLookupSocket socket;
		ServiceMap services;

		public:
		ServiceLookup::ServiceLookup(zmq::context_t & context);
	};

	extern "C" ServiceLookup * createServiceLookup(zmq::context_t & context) {
		return new ServiceLookup(context);
	}

	extern "C" void destroyServiceLookup(Module * servicelookup) {
		delete dynamic_cast<ServiceLookup *>(servicelookup);
	}

	extern "C" int versionServiceLookup() {
		return 1;
	}
	
/* Close namespaces */
			}
		}
	}
}

#endif
