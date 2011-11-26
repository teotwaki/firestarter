
#ifndef __MAIN_H
#define __MAIN_H

#include <libconfig.h++>
#include <iostream>
#include <vector>
#include <string>
#include <boost/foreach.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <dlfcn.h>

#include "log.h"
#include "helper.h"
#include "module.h"

// Prettify BOOST's foreach.
#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_FOREACH_REVERSE

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

DECLARE_LOG(mainLog, "main");

#endif
