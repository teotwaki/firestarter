#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ModuleManager
#include <boost/test/unit_test.hpp>

#include <libconfig.h++>
#include "src/fs/modulemanager.h"

BOOST_AUTO_TEST_CASE( constructor_test ) {
	using namespace firestarter::ModuleManager;
	ModuleManager m0 = ModuleManager(libconfig::Config());
	
	BOOST_CHECK( m0.is_initialised() );
	BOOST_CHECK( m0.getModulePath().length() > (size_t) 0 );
}

