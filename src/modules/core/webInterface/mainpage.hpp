
#ifndef __MAINPAGE_H
#define __MAINPAGE_H

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
