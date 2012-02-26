
#ifndef __WEBSERVER_H
#define __WEBSERVER_H

#include "module.h"
#include "log.h"

#include <Wt/WServer>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebServer {

	class WebServer : public firestarter::module::RunnableModule {
		private:
		Wt::WServer server;
	
		public:
		WebServer(zmq::context_t & context);
		virtual void run();
		virtual void setup();
		virtual void shutdown();
	};
	
	extern "C" WebServer * createWebServer(zmq::context_t & context) {
		return new WebServer(context);
	}
	
	extern "C" void destroyWebServer(Module * webserver) {
		delete reinterpret_cast<WebServer *>(webserver);
	}
	
	extern "C" int versionWebServer() {
		return 1;
	}

/* Close namespaces */
			}
		}
	}
}

#endif
