#include "servicelookup.h"

DECLARE_MODULE_LOG(ServiceLookup);

using namespace firestarter::module::core::ServiceLookup;

ServiceLookup::ServiceLookup(zmq::context_t & context) : RunnableModule(context) {
	LOG_INFO(logger, "ServiceLookup object being created.");
}
