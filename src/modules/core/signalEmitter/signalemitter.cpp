#include "signalemitter.h"

DECLARE_LOG(firestarter::module::core::SignalEmitter::logger, "module.core.SignalEmitter");

using namespace firestarter::module::core::SignalEmitter;

SignalEmitter::SignalEmitter() {
	set_logfile_name("SignalEmitter");
	LOG_DEBUG(logger, "SignalEmitter being created.");
}

void SignalEmitter::beforeFilter() {
	LOG_DEBUG(logger, "SignalEmitter::beforeFilter() called.");
}

void SignalEmitter::setup() {
	LOG_INFO(logger, "SignalEmitter being set up.");
}

void SignalEmitter::afterFilter() {
	LOG_DEBUG(logger, "SignalEmitter::afterFilter() called.");
	sigObjectCreation();
}
