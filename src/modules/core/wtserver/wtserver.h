
#ifndef __WTSERVER_H
#define __WTSERVER_H

#include "module.h"
#include "log.h"

#include <Wt/WServer>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WtServer {

	class WtServer : public firestarter::module::RunnableModule {
		private:
		Wt::WServer server;
	
		public:
		WtServer(zmq::context_t & context);
		virtual void run();
		virtual void setup();
		virtual void shutdown();
	};
	
	extern "C" WtServer * createWtServer(zmq::context_t & context) {
		return new WtServer(context);
	}
	
	extern "C" void destroyWtServer(Module * wtserver) {
		delete reinterpret_cast<WtServer *>(wtserver);
	}
	
	extern "C" int versionWtServer() {
		return 1;
	}

/* Close namespaces */
			}
		}
	}
}

#endif
