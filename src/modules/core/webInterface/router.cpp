#include "router.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

std::map<std::string, Router::PageFactory> Router::page_factory;

bool Router::response() {
	using namespace firestarter::common::WebWidgets::Pages;

	WebPage * page = this->page_factory["main"]();
	std::string contents = this->page_factory["main"]()->render();
	LOG_DEBUG(logger, "Page contents: " << contents);
	this->out << contents;

	return true;
}
