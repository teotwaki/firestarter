
#ifndef __MODULE_H
#define __MODULE_H

#include <string>
#include <boost/tr1/unordered_map.hpp>

#include "helper.h"

class Module;

                             /* name */        /* version */
typedef boost::unordered_map<std::string, std::pair<int, Module *> > ComponentMap;
                                                       /* Module */
typedef ComponentMap BehaviourMap;

class Module {
	protected:
	/* Properties */
	ComponentMap components;
	BehaviourMap behaviours;

	public:
	virtual void beforeFilter() = 0; /* pure virtual */
	virtual void setup() = 0; /* pure virtual */
	virtual void afterFilter() = 0; /* pure virtual */
	inline const ComponentMap & getComponents() const { return components; };
	inline const BehaviourMap & getBehaviours() const { return behaviours; };

	inline void addComponent(std::string name, int version, Module * module = NULL) { components[name] = std::make_pair(version, module); };
	inline void addBehaviour(std::string name, int version, Module * module = NULL) { behaviours[name] = std::make_pair(version, module); };
};

typedef Module * create_module();
typedef void destroy_module(Module *);
typedef int module_version();

#endif
