#include "webinterface.h"

DECLARE_MODULE_LOG(WebInterface);

using namespace firestarter::module::core::WebInterface;

WebInterface::WebInterface(zmq::context_t & context) : RunnableModule(context) {
	LOG_INFO(logger, "WebInterface object being created.");
}

void WebInterface::run() {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "Running the WebInterface's main function.");

	try {
		Router::registerPage<AdminPage>("main");
		LOG_DEBUG(logger, "Creating fcgi object.");
		Fastcgipp::Manager<Router> fcgi(this->socket_fd);
		LOG_DEBUG(logger, "Calling fcgi.handler().");
		fcgi.handler();
	}
	catch (std::exception & e) {
		LOG_ERROR(logger, "Exception caught while running fcgi handler:");
		LOG_ERROR(logger, e.what());
	}
}

void WebInterface::setup() {
	std::string socket_path = "/tmp/fstest.socket";

	LOG_INFO(logger, "WebInterface being set up.");
	LOG_DEBUG(logger, "Creating Unix Domain Socket at " << socket_path);

	int len;
	struct sockaddr_un local, remote;

	int old_umask = ::umask(~(0777&0777));

    if ((this->socket_fd = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		LOG_ERROR(logger, "Couldn't socket().");
		return;
    }

	local.sun_family = AF_UNIX;
	::strcpy(local.sun_path, socket_path.c_str());
	::unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (::bind(this->socket_fd, (struct sockaddr *)&local, len) == -1) {
		LOG_ERROR(logger, "Couldn't bind().");
		return;
	}

	::umask(old_umask);

	if (::listen(this->socket_fd, 5) == -1) {
		LOG_ERROR(logger, "Couldn't listen().");
		return;
	}
}

void WebInterface::shutdown() {
	::unlink("/tmp/fstest.socket");
}
