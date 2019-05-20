/***************************************************************************
 *   Copyright (C) 2003-2011 by Tux Football development team              *
 *   Authors: Christoph Brill <egore911@egore911.de>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef TUXFOOTBALL_LOGGER
#define TUXFOOTBALL_LOGGER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

class Logger {
public:
	enum LogLevel {
		Debug, Info, Warn, Error
	};

	static LogLevel level();
	static void setLevel(LogLevel level);
	static void log(const std::ostringstream& oss);
	static void err(const std::ostringstream& oss);

private:
	static LogLevel m_level;
};

#define DEBUG(text) {std::ostringstream oss; if (Logger::Debug >= Logger::level()) Logger::log((std::ostringstream&)(oss << "[DEBUG] " << text));}
#define INFO(text) {std::ostringstream oss; if (Logger::Info >= Logger::level()) Logger::log((std::ostringstream&)(oss << "[INFO] " << text));}
#define WARN(text) {std::ostringstream oss; if (Logger::Warn >= Logger::level()) Logger::err((std::ostringstream&)(oss << "[WARN] " << text));}
#define ERROR(text) {std::ostringstream oss; if (Logger::Error >= Logger::level()) Logger::err((std::ostringstream&)(oss << "[ERROR] " << text));}

#endif /* TUXFOOTBALL_LOGGER */
