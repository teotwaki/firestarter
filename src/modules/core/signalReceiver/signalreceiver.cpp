#include "signalreceiver.h"

namespace firestarter { namespace module { namespace core { namespace SignalReceiver {
	DECLARE_LOG(logger, "module.core.SignalReceiver");
} } } }

using namespace firestarter::module::core::SignalReceiver;

SignalReceiver::SignalReceiver() {
	set_logfile_name("SignalReceiver");
	LOG_DEBUG(logger, "SignalReceiver being created.");
}

void SignalReceiver::beforeFilter() {
	LOG_DEBUG(logger, "SignalReceiver::beforeFilter() called.");
	addComponent("SignalEmitter", 1);
}

void SignalReceiver::setup() {
	using firestarter::module::core::SignalEmitter::SignalEmitter;
	LOG_INFO(logger, "SignalReceiver being set up.");
	SignalEmitter * sigEmitter = dynamic_cast<SignalEmitter *>(components["SignalEmitter"].second);
	sigEmitter->addSignalObjectCreation(boost::bind(&SignalReceiver::onObjectCreation, this));
}

void SignalReceiver::afterFilter() {
	LOG_DEBUG(logger, "SignalReceiver::afterFilter() called.");
}

void SignalReceiver::onObjectCreation() {
	LOG_INFO(logger, "Received signal!");
}
