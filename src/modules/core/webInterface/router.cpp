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

#include "router.hpp"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

std::map<std::string, Router::PageFactory> Router::page_factory;

bool Router::response() {
	using namespace firestarter::common::WebWidgets::Pages;

	auto page = this->instantiate("blank");
	std::string contents = page->render();
	LOG_DEBUG(logger, "Page contents: " << contents);
	this->out << contents;

	return true;
}
