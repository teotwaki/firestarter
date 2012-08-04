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
