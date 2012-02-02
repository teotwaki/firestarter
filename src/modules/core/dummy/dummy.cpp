#include "dummy.h"

namespace firestarter { namespace module { namespace core { namespace Dummy {
	DECLARE_LOG(logger, "module.core.Dummy");
} } } }

using namespace firestarter::module::core::Dummy;

Dummy::Dummy() {
	set_logfile_name("Dummy");
	LOG_DEBUG(logger, "Dummy being created.");
}

void Dummy::setup() {
	LOG_INFO(logger, "Dummy being set up.");
}
