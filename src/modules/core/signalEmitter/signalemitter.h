
#ifndef __SIGNALEMITTER_H
#define __SIGNALEMITTER_H

#include <boost/signals2.hpp>

#include "module.h"
#include "log.h"
#include "helper.h"

namespace firestarter {
	namespace module {
		namespace core {
			namespace SignalEmitter {

	DECLARE_LOG(sigELog, "module.core.SignalEmitter");
	
	class SignalEmitter : Module {
	
		public:
		SignalEmitter();
		void beforeFilter();
		void setup();
		void afterFilter();
	
		typedef boost::signals2::signal<void ()> ObjectCreationSignal;
		inline void addSignalObjectCreation(const ObjectCreationSignal::slot_type& slot) const { sigObjectCreation.connect(slot); };
	
		private:
		mutable ObjectCreationSignal sigObjectCreation;
	};
	
	extern "C" SignalEmitter * createSignalEmitter() {
		return new SignalEmitter;
	}
	
	extern "C" void destroySignalEmitter(Module * signalemitter) {
		delete signalemitter;
	}
	
	extern "C" int versionSignalEmitter() {
		return 1;
	}

/* Close the namespace */
			}
		}
	}
}

#endif
