#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ModuleManager
#include <boost/test/unit_test.hpp>

#include <libconfig.h++>
#include "src/fs/modulemanager.h"

/* Fixtures */
struct EmptyConfig {
	libconfig::Config config;
};

BOOST_FIXTURE_TEST_CASE(constructor_test, EmptyConfig) {
	using namespace firestarter::ModuleManager;
	ModuleManager m0 = ModuleManager(config);
	
	BOOST_CHECK(m0.is_initialised());
	BOOST_CHECK(not m0.getModulePath().empty());
	BOOST_CHECK(m0.getModulePath() == "/usr/lib/firestarter");
	BOOST_CHECK(m0.getModuleList().empty());

}

BOOST_FIXTURE_TEST_CASE(robustness_test, EmptyConfig) {
	using namespace firestarter::ModuleManager;
	using namespace firestarter::exception;

	ModuleManager m1 = ModuleManager(config);

	BOOST_CHECK_THROW(m1.getModule("this should throw an exception"), ModuleNotFoundException);
	BOOST_CHECK_THROW(m1.getModule(NULL), std::logic_error);
	BOOST_CHECK_THROW(m1.getModule(""), ModuleNotFoundException);

	BOOST_CHECK_THROW(m1.loadModule("this should throw an exception"), ModuleNotFoundException);
	BOOST_CHECK_THROW(m1.loadModule(NULL), std::logic_error);
	BOOST_CHECK_THROW(m1.loadModule(""), ModuleNotFoundException);
}

struct FakeConfig {
	FakeConfig() : fakepath("/does/not/exist") {
		using namespace libconfig;

		Setting & root = config.getRoot();
		root.add("application", Setting::TypeGroup);

		Setting & app = root["application"];
		app.add("logdir_path", Setting::TypeString) = fakepath + "logs";
		app.add("module_path", Setting::TypeString) = fakepath;
		app.add("modules", Setting::TypeArray);

		Setting & mods = app["modules"];
		mods.add(Setting::TypeString) = "Dummy";
	}
	~FakeConfig() { }
	libconfig::Config config;
	std::string fakepath;
};

BOOST_FIXTURE_TEST_CASE(constructor_test_2, FakeConfig) {
	using namespace firestarter::ModuleManager;

	ModuleManager m2 = ModuleManager(config);

	BOOST_CHECK(m2.is_initialised());
	BOOST_CHECK(not m2.getModulePath().empty());
	BOOST_CHECK(m2.getModulePath() == fakepath);
	BOOST_CHECK(not m2.getModuleList().empty());
	BOOST_CHECK_EQUAL(m2.getModuleList().size(), 1);
}
