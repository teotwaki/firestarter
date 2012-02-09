
#ifndef __DUMMY_H
#define __DUMMY_H

#include "module.h"
#include "log.h"

namespace firestarter {
	namespace module {
		namespace core {
			namespace Dummy {

	class Dummy : public Module {
		private:
	
		public:
		Dummy(zmq::context_t * context);
		void setup();
	};
	
	extern "C" Dummy * createDummy(zmq::context_t * context) {
		return new Dummy(context);
	}
	
	extern "C" void destroyDummy(Module * dummy) {
		delete reinterpret_cast<Dummy *>(dummy);
	}
	
	extern "C" int versionDummy() {
		return 1;
	}

/* Close namespaces */
			}
		}
	}
}

#endif
