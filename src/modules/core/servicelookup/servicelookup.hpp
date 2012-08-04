
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
