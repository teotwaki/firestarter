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

#include "webwidgets/basepage.hpp"

namespace firestarter { namespace common { namespace WebWidgets { namespace Pages {
	DECLARE_LOG(logger, "firestarter.common.WebWidgets.Pages");
} } } }

using namespace firestarter::common::WebWidgets::Pages;

WebPage::Sessions WebPage::sessions(3600, 3600);

std::string WebPage::render() {
	LOG_DEBUG(logger, "WebPage::render() called.");
	LOG_DEBUG(logger, "Calling this->response().");
	this->response();
	LOG_DEBUG(logger, "Page built, rendering output.");
	return this->headers.render() + this->html.render();
}
