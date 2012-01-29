
#ifndef __MAIN_H
#define __MAIN_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include <libconfig.h++>
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>

#if HAVE_LTDL_H
	#include <ltdl.h>
#endif

#include "helper.h"
#include "modulemanager.h"

using namespace std;

typedef boost::tuple<string, int, void *, create_module *, destroy_module *> ModuleTuple;
typedef boost::unordered_map<string, ModuleTuple> ModuleMap;
//typedef boost::unordered_map<string, boost::tuple<string, int, void *, create_module *, destroy_module *> > ModuleMap;
/*                             ^- name              ^- path |    |       ^- factory       ^- delete           ^- Type name
                                                            |    ^- file handle
                                                            ^-version

	See src/common/module.h for more information concerning create_module and destroy_module.
*/

typedef boost::unordered_map<string, ModuleMap> ModuleDependencyMap;
/*                           ^- name |          ^- Type name
                                     ^- List of children modules
*/

#endif
