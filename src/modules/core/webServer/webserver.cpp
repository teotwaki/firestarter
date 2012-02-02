#include "webserver.h"

namespace firestarter { namespace module { namespace core { namespace WebServer {
	DECLARE_LOG(logger, "module.core.WebServer");
} } } }

using namespace firestarter::module::core::WebServer;

WebServer::WebServer() : server("firestarter") {
	LOG_INFO(logger, "WebServer being created.");
}

void WebServer::setup() {
	LOG_INFO(logger, "WebServer being set up.");
	LOG_DEBUG(logger, "Attempting to set the web server's configuration.");
	try {
		this->server.setServerConfiguration(0, NULL, "/etc/wt/wt_config.xml");
		this->server.start();
	}

	catch (Wt::WServer::Exception & wex) {
		LOG_ERROR(logger, "Exception raised while starting the server:");
		LOG_ERROR(logger, wex.what());
	}
	catch (std::exception & sex) {
		LOG_ERROR(logger, "Exception raised while starting the server:");
		LOG_ERROR(logger, sex.what());
	}
}

void WebServer::shutdown() {
	LOG_INFO(logger, "WebServer shutting down.");
	try {
		this->server.stop();
	}

	catch (Wt::WServer::Exception & wex) {
		LOG_ERROR(logger, "Exception raised while shutting down the server:");
		LOG_ERROR(logger, wex.what());
	}
	catch (std::exception & sex) {
		LOG_ERROR(logger, "Exception raised while shutting down the server:");
		LOG_ERROR(logger, sex.what());
	}
}
