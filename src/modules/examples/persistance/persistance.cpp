#include "persistance.hpp"

DECLARE_MODULE_LOG(Persistance);

using namespace firestarter::module::examples::Persistance;

Persistance::Persistance(zmq::context_t & context) : RunnableModule(context) {
	using namespace firestarter::common;
	LOG_INFO(logger, "Persistance object being created.");


	// First, we need to connect to the database, and ensure the table exists.
	// This is done by allocating an objet of the class we wish to persist, and reserving the required size for strings
	// For example, a std::string with a capacity of 100 == VARCHAR(100).
	// Or, a std::string with a capacity of 20000 == TEXT.
	// This step isn't required for most databases (MySQL, PostgreSQL), where the table can be expected to exist
	// However, in the case of sqlite (used in this example), the database can not have existed prior to us calling it.
	{
		Person p;
		p.first_name.reserve(30);
		p.last_name.reserve(30);
		p.city.reserve(30);
		p.street_address.reserve(50);
		p.postcode.reserve(20);

		std::string connection_string = "sqlite3://dbname=/tmp/foobar.db";
		LOG_DEBUG(logger, "Connecting to the database at:" << connection_string);

		Persist<Person>::connect(connection_string);
		LOG_DEBUG(logger, "Creating a table of type Person, if it doesn't exist");
		Persist<Person>::setup(p);
	}

	// Create a new person from nothing.
	Person p;
	p.id = 1;
	p.first_name = "Roger";
	p.last_name = "RaBite";
	p.city = "Perpignan";
	p.postcode = "SE1 0UQ";
	p.street_address = "3, Rue Gustave Eiffel\n"
		"66280 Pimpette";

	// Store it in the table
	LOG_DEBUG(logger, "Storing Roger Rabite.");
	Persist<Person>::store(p);

	// Update it in memory
	p.city = "Saleilles";
	// Then commit the changes
	Persist<Person>::commit(p);

	// This example shows that values are overwritten, even if the database contained NULL.
	{
		Person f;
		f.id = 2;
		f.first_name = "Second test";
		f.last_name = "Of the example";
		LOG_DEBUG(logger, "Storing Second test.");
		Persist<Person>::store(f);
	}
	{
		Person f;
		f.id = 200;
		f.first_name = "Not my real name";
		f.city = "This will be overwritten.";
		LOG_DEBUG(logger, "Looking for a person with id == 2");
		Persist<Person>::find(f, Column<Person>().id == 2);
		if (f.id == 2) {
			LOG_DEBUG(logger, "We found a person: " << f.first_name);	
		}
		else {
			LOG_DEBUG(logger, "Whoops, something went wrong, person is still: " << f.first_name);
		}
	}

	// How stepping/pagination works:
	std::vector<Person> people;
	// Simply reserve the maximum amount of results you wish to get:
	people.reserve(100);

	Persist<Person>::find(people, Column<Person>().first_name != "");
	LOG_INFO(logger, "findAll() found " << people.size() << " entries (" << Persist<Person>::count() << " in db)");
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
