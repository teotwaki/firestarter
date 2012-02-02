
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
		Dummy();
		void setup();
	};
	
	extern "C" Dummy * createDummy() {
		return new Dummy;
	}
	
	extern "C" void destroyDummy(Module * dummy) {
		delete dummy;
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
