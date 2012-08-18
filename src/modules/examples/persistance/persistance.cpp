#include "persistance.hpp"

DECLARE_MODULE_LOG(Persistance);

using namespace firestarter::module::examples::Persistance;

Persistance::Persistance(zmq::context_t & context) : RunnableModule(context) {
	using namespace firestarter::common;
	LOG_INFO(logger, "Persistance object being created.");
	Person p;
	p.first_name = "Roger";
	p.last_name = "LaBite";
	Persist<Person>::connect("sqlite3://dbname=/tmp/foobar.db");
	Persist<Person>::store(p);
	Person f = Persist<Person>::find(Column<Person>().id() == 1 || Column<Person>().id() == 2);
	LOG_INFO(logger, "Seem to have found someone:");
	LOG_INFO(logger, "f.id: " << f.id << "; f.first_name: " << f.first_name << "; f.last_name: " << f.last_name);

	std::vector<Person> people;
	people.reserve(2);

	Persist<Person>::findAll(people, Column<Person>().first_name() != "");
	LOG_INFO(logger, "findAll() found:");
	for (Person const & person : people) {
		LOG_INFO(logger, "person.id: " << person.id << "; person.first_name: " << 
			person.first_name << "; person.last_name: " << person.last_name);
	}
}

void Persistance::run() {

}

void Persistance::setup() {

}

void Persistance::shutdown() {

}
