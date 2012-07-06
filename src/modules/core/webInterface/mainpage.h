
#ifndef __MAINPAGE_H
#define __MAINPAGE_H

#include "log.h"

#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class AdminPage : public Fastcgipp::Request<wchar_t> {
		typedef Fastcgipp::Http::Sessions<std::wstring> Sessions;
		static Sessions sessions;
		Sessions::iterator session;

		bool response();
		void handleSession();
		void handleNoSession();
	};

/* Close namespaces */
			}
		}
	}
}

#endif
