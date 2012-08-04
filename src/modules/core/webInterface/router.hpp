
#ifndef FIRESTARTER_ROUTER_HPP
#define FIRESTARTER_ROUTER_HPP

#include "log.hpp"
#include "webwidgets/basepage.hpp"

#include <boost/bind.hpp>
#include <fastcgi++/request.hpp>
#include <boost/functional/factory.hpp>
#include <boost/function.hpp>
#include <map>
#include <memory>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class Router : public Fastcgipp::Request<char> {
		protected:
		typedef boost::function<firestarter::common::WebWidgets::Pages::WebPage * 
									(/*const Fastcgipp::Http::Environment<char> & environment*/)> PageFactory;
		static std::map<std::string, PageFactory> page_factory;
		std::unique_ptr<firestarter::common::WebWidgets::Pages::WebPage> instantiate(std::string const & name) {
			return std::unique_ptr<firestarter::common::WebWidgets::Pages::WebPage>(this->page_factory[name]());
		}

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
