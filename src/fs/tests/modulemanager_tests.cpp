/*
 * Copyright (C) 2012  Sebastian Lauwers <sebastian.lauwers@gmail.com>
 *
 * This file is part of Firestarter.
 *
 * Firestarter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firestarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ModuleManager
#include <boost/test/unit_test.hpp>

#include <libconfig.h++>
#include "src/fs/modulemanager.hpp"
#include "src/common/exceptions.hpp"

BOOST_AUTO_TEST_CASE(constructor_test) {
	using namespace firestarter::ModuleManager;

	libconfig::Config c;

	BOOST_CHECK_THROW(ModuleManager m0 = ModuleManager(c), firestarter::exception::InvalidConfigurationException);
}

struct EmptyConfig {
	EmptyConfig() {
		using namespace libconfig;

		Setting & root = config.getRoot();
		root.add("application", Setting::TypeGroup);
		root["application"].add("modules", Setting::TypeArray);
	}
	libconfig::Config config;
};

BOOST_FIXTURE_TEST_CASE(constructor_test_emptyconfig, EmptyConfig) {
	using namespace firestarter::ModuleManager;
	ModuleManager m1 = ModuleManager(config);

	BOOST_CHECK(m1.isInitialised());
	BOOST_CHECK(not m1.getModulePath().empty());
	BOOST_CHECK(m1.getModulePath() == LIBDIR);
	BOOST_CHECK(m1.getModuleList()->empty());
}

BOOST_FIXTURE_TEST_CASE(robustness_test, EmptyConfig) {
	using namespace firestarter::ModuleManager;
	using namespace firestarter::exception;

	ModuleManager m2 = ModuleManager(config);

	BOOST_CHECK_THROW(m2.getModuleInfo("this should throw an exception"), ModuleNotFoundException);
	BOOST_CHECK_THROW(m2.getModuleInfo(NULL), std::logic_error);
	BOOST_CHECK_THROW(m2.getModuleInfo(""), ModuleNotFoundException);

	BOOST_CHECK_THROW(m2.loadModule("this should throw an exception"), ModuleNotFoundException);
	BOOST_CHECK_THROW(m2.loadModule(NULL), std::logic_error);
	BOOST_CHECK_THROW(m2.loadModule(""), ModuleNotFoundException);
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

BOOST_FIXTURE_TEST_CASE(constructor_test_fakeconfig, FakeConfig) {
	using namespace firestarter::ModuleManager;

	ModuleManager m3 = ModuleManager(config);

	BOOST_CHECK(m3.isInitialised());
	BOOST_CHECK(not m3.getModulePath().empty());
	BOOST_CHECK(m3.getModulePath() == fakepath);
	BOOST_CHECK(not m3.getModuleList()->empty());
	BOOST_CHECK_EQUAL(m3.getModuleList()->size(), 1);
}
