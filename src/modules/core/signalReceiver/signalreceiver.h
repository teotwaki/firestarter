
#ifndef __SIGNALRECEIVER_H
#define __SIGNALRECEIVER_H

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include "module.h"
#include "log.h"
#include "core/signalEmitter/signalemitter.h"

namespace firestarter {
	namespace module {
		namespace core {
			namespace SignalReceiver {

	class SignalReceiver : Module {
		private:

		public:
		SignalReceiver();
		void beforeFilter();
		void setup();
		void afterFilter();
	
		void onObjectCreation();
	};
	
	extern "C" SignalReceiver * createSignalReceiver() {
		return new SignalReceiver;
	}
	
	extern "C" void destroySignalReceiver(Module * signalreceiver) {
		delete signalreceiver;
	}
	
	extern "C" int versionSignalReceiver() {
		return 1;
	}

/* Close the namespace */
			}
		}
	}
}

#endif
