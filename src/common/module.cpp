#include "module.h"

namespace firestarter { namespace module {
	DECLARE_EXTERN_LOG(logger);
} }

using namespace firestarter::module;

void RunnableModule::shutdown() { 
	LOG_WARN(logger, "shutdown() not implemented in RunnableModule (this = " << this << ")!");
}

void RunnableModule::restart() { 
	LOG_WARN(logger, "restart() not implemented in RunnableModule (this = " << this << ")!");
};
