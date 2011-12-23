#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

namespace firestarter {
	namespace exception {

class ModuleNotFoundException : public std::exception {
	virtual const char * what() const throw() {
		return "Requested module could not be found";
	}
};

class ModuleNotLoadableException : public ModuleNotFoundException {
	virtual const char * what() const throw() {
		return "Requested module could not be loaded";
	}
};

class MissingDependencyException : public ModuleNotLoadableException {
	virtual const char * what() const throw() {
		return "Module dependency could not be found";
	}
};

/* Closing the namespace */
	}
}

#endif
