#include "router.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

std::map<std::string, Router::PageFactory> Router::page_factory;

bool Router::response() {
	using namespace firestarter::common::WebWidgets::Pages;

	LOG_ERROR(logger, "page_factory::size: " << this->page_factory.size());
	WebPage * page = this->page_factory["main"]();
	LOG_ERROR(logger, "pointer: " << page);
	this->out << this->page_factory["main"]()->render();

	return true;
}
