#include "persistance.hpp"

DECLARE_MODULE_LOG(Persistance);

using namespace firestarter::module::examples::Persistance;

Persistance::Persistance(zmq::context_t & context) : RunnableModule(context) {
	using namespace firestarter::common;
	LOG_INFO(logger, "Persistance object being created.");
	Person p;
	p.first_name = "Roger";
	p.last_name = "RaBite";
	p.city = "Perpignan";
	p.postcode = "SE1 0UQ";
	p.street_address = "3, Rue Gustave Eiffel\n"
		"66280 Pimpette";
	Persist<Person>::connect("sqlite3://dbname=/tmp/foobar.db");
	Persist<Person>::store(p);
	Person f;
	f.last_name = "Booze";
	f.city = "Barf";
	Persist<Person>::find(f, Column<Person>().id() == 1 || Column<Person>().id() == 2);

	LOG_INFO(logger, "Seem to have found someone:");
	LOG_INFO(logger, "f.id: " << f.id << "; f.first_name: " << f.first_name << "; f.last_name: " << f.last_name << 
		"; f.city: " << f.city);

	std::vector<Person> people;
	people.reserve(100);

	Persist<Person>::find(people, Column<Person>().first_name() != "");
	LOG_INFO(logger, "findAll() found:");
	for (Person const & person : people) {
		LOG_INFO(logger, "person.id: " << person.id << "; person.first_name: " << 
			person.first_name << "; person.last_name: " << person.last_name);
	}

/*	for (int i = 3; i < 100; i++) {
		p.id = i;
		Persist<Person>::store(p);
	}*/
}

void Persistance::run() {

}

void Persistance::setup() {

}

void Persistance::shutdown() {

}
