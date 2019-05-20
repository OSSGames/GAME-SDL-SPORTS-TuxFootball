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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "fontmanager.h"

#include <iostream>

#include "surfacemanager.h"
#include "const.h"
#include "logger/logger.h"

std::map<std::string, ManagedFont> FontManager::m_fonts;

SFont *FontManager::load(std::string filename)
{
	// FIXME: Dummy, we need the SDL_PixelFormat
	return 0;
}

SFont *FontManager::load(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha)
{
	if(m_fonts[filename].refCount == 0) {
		return add(format, filename, colorKey, alpha);
	} else {
		m_fonts[filename].refCount++;
		return m_fonts[filename].font;
	}
}

void FontManager::release(SFont *font)
{
	std::map<std::string, ManagedFont>::iterator itt;

	itt = m_fonts.begin();

	while(itt!= m_fonts.end()) {
		if( (*itt).second.font == font) {
			--((*itt).second.refCount);
			if(! (*itt).second.refCount) {
				// NOTE: This only works because we changed SFont_FreeFont NOT
				// to release the surface
				delete ((*itt).second.font);
				SurfaceManager::instance()->release((*itt).second.surface);
				m_fonts.erase(itt);
			}
			return;
			
		}
		++itt;
	}

	ERROR("Tried to free a font, could not be found.");
}

SFont *FontManager::add(std::string filename)
{
	// FIXME: Dummy, we need the SDL_PixelFormat
	return 0;
}

SFont *FontManager::add(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha)
{
	SDL_Surface* fontSurface = SurfaceManager::instance()->load(format, filename, colorKey, alpha);
	SFont* font = new SFont(fontSurface);
	
	if(!font) {
		ERROR("Tried to add font \"" << filename.c_str() << "\" but failed");
		return 0;
	}

	m_fonts[filename].font = font;
	m_fonts[filename].refCount = 1;
	m_fonts[filename].surface = fontSurface;

	return m_fonts[filename].font;
}

