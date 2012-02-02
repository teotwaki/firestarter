
#ifndef __WEBSERVER_H
#define __WEBSERVER_H

#include "module.h"
#include "log.h"

#include <Wt/WServer>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebServer {



	class WebServer : public Module {
		private:
		Wt::WServer server;
	
		public:
		WebServer();
		void setup();
		void shutdown();
	};
	
	extern "C" WebServer * createWebServer() {
		return new WebServer;
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
