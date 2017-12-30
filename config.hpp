/*
 * Copyright (C) 2011-2017 Tycho Softworks.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <QDebug>

#if defined(Q_OS_MAC)
#define CONFIG_FROM "tychosoft.com", "textseeker"
#elif defined(Q_OS_WIN)
#define CONFIG_FROM "Tycho Softworks", "TextSeeker"
#else
#define CONFIG_FROM "tychosoft.com", "textseeker"
#endif

/*!
 * Basic application configuration for each platform.  This is used to select
 * the correct style sheet and config path.  I use style sheets to adjust the
 * visual appearence of some ui elements to better blend with different native
 * platforms (windows, desktop linux, mac osx).  The actual per-platform style
 * sheets are embedded in the applications qrc.
 * \file config.hpp
 */

#endif
