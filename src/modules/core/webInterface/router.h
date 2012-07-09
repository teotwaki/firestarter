
#ifndef __ROUTER_H
#define __ROUTER_H

#include "log.h"
#include "webwidgets/basepage.h"

#include <boost/bind.hpp>
#include <fastcgi++/request.hpp>
#include <boost/functional/factory.hpp>
#include <boost/function.hpp>
#include <map>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class Router : public Fastcgipp::Request<char> {
		protected:
		typedef boost::function<firestarter::common::WebWidgets::Pages::WebPage * 
									(/*const Fastcgipp::Http::Environment<char> & environment*/)> PageFactory;
		static std::map<std::string, PageFactory> page_factory;

		bool response();

		public:
		template <class T> static void registerPage(std::string name) {
			Router::page_factory[name] = boost::factory<T*>();
		}
		
	};

/* Close namespaces */
			}
		}
	}
}

#endif
