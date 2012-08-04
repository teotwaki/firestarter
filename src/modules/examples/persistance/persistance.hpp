
#ifndef FIRESTARTER_PERSISTANCE_HPP
#define FIRESTARTER_PERSISTANCE_HPP

#include "module.hpp"
#include "log.hpp"

#include "mirror/mirror.hpp"

#include "namespace-person.meta.hpp"
#include "class-person.meta.hpp"

namespace firestarter {
	namespace module {
		namespace examples {
			namespace Persistance {

	class Persistance : public firestarter::module::RunnableModule {

		public:
		Persistance(zmq::context_t & context);
		virtual void run();
		virtual void setup();
		virtual void shutdown();
	};

	extern "C" Persistance * createPersistance(zmq::context_t & context) {
		return new Persistance(context);
	}

	extern "C" void destroyPersistance(Module * persistance) {
		delete dynamic_cast<Persistance *>(persistance);
	}

	extern "C" int versionPersistance() {
		return 1;
	}

/* Close namespaces */
			}
		}
	}
}

#endif
