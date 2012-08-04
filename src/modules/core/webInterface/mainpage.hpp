
#ifndef FIRESTARTER_MAINPAGE_HPP
#define FIRESTARTER_MAINPAGE_HPP

#include "log.hpp"
#include "webwidgets/basepage.hpp"
#include "webwidgets/htmltemplates.hpp"

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class AdminPage : public firestarter::common::WebWidgets::Pages::WebPage {
		bool response();
	};

/* Close namespaces */
			}
		}
	}
}

#endif
