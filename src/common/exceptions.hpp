#ifndef FIRESTARTER_EXCEPTIONS_HPP
#define FIRESTARTER_EXCEPTIONS_HPP

#include <exception>

namespace firestarter {
	namespace exception {

class Exception : public std::exception {
	public:
	Exception(const char * message = NULL) throw() : message(message) { }
	virtual const char * what() const throw() {
		return message;
	}
	private:
	const char * message;
};

class ModuleNotFoundException : public Exception {
	public:
	ModuleNotFoundException(const char * message = "Requested module could not be found") throw() : Exception(message) { }
};

class ModuleNotLoadableException : public ModuleNotFoundException {
	public:
	ModuleNotLoadableException(const char * message = "Requested module could not be loaded") throw() : ModuleNotFoundException(message) { }
};

class MissingDependencyException : public ModuleNotLoadableException {
	public:
	MissingDependencyException(const char * message = "Module dependency could not be found") throw() : ModuleNotLoadableException(message) { }
};

class InvalidConfigurationException : public Exception {
	public:
	InvalidConfigurationException(const char * message = "Configuration file is not valid") throw() : Exception(message) { }
};

/* Closing the namespace */
	}
}

#endif
