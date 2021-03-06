
#ifndef FIRESTARTER_PERSON_HPP
#define FIRESTARTER_PERSON_HPP

#include <string>
#include <mirror/meta_class.hpp>
#include <mirror/meta_enum.hpp>
#include <mirror/pre_registered/type/native.hpp>

namespace firestarter {
	namespace module {
		namespace examples {
			namespace Persistance {

	struct Person {
		unsigned int id;
		std::string first_name;
		std::string last_name;
		std::string street_address;
		std::string city;
		std::string postcode;
	};

/* Close namespaces */
			}
		}
	}
}

#endif
