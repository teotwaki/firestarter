
#ifndef __MAINPAGE_H
#define __MAINPAGE_H

#include "log.h"
#include "webwidgets/basepage.h"
#include "webwidgets/htmltemplates.h"

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
