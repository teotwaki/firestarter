#include "signalemitter.h"

using namespace firestarter::module::core::SignalEmitter;

SignalEmitter::SignalEmitter() {
	set_logfile_name("SignalEmitter");
	LOG_DEBUG(sigELog, "SignalEmitter being created.");
}

void SignalEmitter::beforeFilter() {
	LOG_DEBUG(sigELog, "SignalEmitter::beforeFilter() called.");
}

void SignalEmitter::setup() {
	LOG_INFO(sigELog, "SignalEmitter being set up.");
}

void SignalEmitter::afterFilter() {
	LOG_DEBUG(sigELog, "SignalEmitter::afterFilter() called.");
	sigObjectCreation();
}
