#include "signalreceiver.h"

using namespace firestarter::module::core::SignalReceiver;

SignalReceiver::SignalReceiver() {
	set_logfile_name("SignalReceiver");
	LOG_DEBUG(sigRLog, "SignalReceiver being created.");
}

void SignalReceiver::beforeFilter() {
	LOG_DEBUG(sigRLog, "SignalReceiver::beforeFilter() called.");
	addComponent("SignalEmitter", 1);
}

void SignalReceiver::setup() {
	using firestarter::module::core::SignalEmitter::SignalEmitter;
	LOG_INFO(sigRLog, "SignalReceiver being set up.");
	SignalEmitter * sigEmitter = dynamic_cast<SignalEmitter *>(components["SignalEmitter"].second);
	sigEmitter->addSignalObjectCreation(boost::bind(&SignalReceiver::onObjectCreation, this));
}

void SignalReceiver::afterFilter() {
	LOG_DEBUG(sigRLog, "SignalReceiver::afterFilter() called.");
}

void SignalReceiver::onObjectCreation() {
	LOG_INFO(sigRLog, "Received signal!");
}
