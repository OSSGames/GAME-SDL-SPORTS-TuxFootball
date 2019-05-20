/***************************************************************************
 *   Copyright (C) 2003-2011 by Tux Football development team              *
 *   Authors: Jason Wood <jasonwood@blueyonder.co.uk>                      *
 *            Christoph Brill <egore911@egore911.de>                       *
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

#ifndef TUXFOOTBALL_FONTMANAGER
#define TUXFOOTBALL_FONTMANAGER

#include "resourcemanager.h"

#include <string>
#include <map>

#include "Font.hpp"

struct ManagedFont {
	SFont *font;
	int refCount;
	SDL_Surface *surface;
};

/** Manages fonts, makes sure that they are only loaded once and are deleted
 * when no longer needed. */
class FontManager : public ResourceManager<SFont, FontManager> {
public:
	virtual SFont *load(std::string filename);
	virtual SFont *load(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha);
	virtual void release(SFont *font);
protected:
	virtual SFont *add(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha);
	virtual SFont *add(std::string filename);
private:
	static std::map<std::string, ManagedFont> m_fonts;
};

#endif /* TUXFOOTBALL_FONTMANAGER */
