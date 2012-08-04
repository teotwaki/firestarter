
#ifndef __WEBINTERFACE_H
#define __WEBINTERFACE_H

#include "module.hpp"
#include "log.hpp"
#include "router.hpp"
#include "mainpage.hpp"
#include "blankpage.hpp"

#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class WebInterface : public firestarter::module::RunnableModule {
		private:
		int socket_fd;

		public:
		WebInterface(zmq::context_t & context);
		virtual void run();
		virtual void setup();
		virtual void shutdown();
	};

	extern "C" WebInterface * createWebInterface(zmq::context_t & context) {
		return new WebInterface(context);
	}

	extern "C" void destroyWebInterface(Module * webinterface) {
		delete dynamic_cast<WebInterface *>(webinterface);
	}

	extern "C" int versionWebInterface() {
		return 1;
	}

/* Close namespaces */
			}
		}
	}
}

#endif
