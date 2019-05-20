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

#include "surfacemanager.h"
#include "const.h"
#include "logger/logger.h"

#include <iostream>
#include <SDL_image.h>

std::map<std::string, ManagedSurface> SurfaceManager::m_surfaces;

SDL_Surface *SurfaceManager::load(std::string filename) {
	// FIXME: Dummy, we need the SDL_PixelFormat
	return 0;
}

SDL_Surface *SurfaceManager::load(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha)
{
	if(m_surfaces[filename].refCount == 0) {
		return add(format, filename, colorKey, alpha);
	} else {
		m_surfaces[filename].refCount++;
		return m_surfaces[filename].surface;
	}
}

void SurfaceManager::release(SDL_Surface *surface)
{
	std::map<std::string, ManagedSurface>::iterator itt;

	itt = m_surfaces.begin();

	while(itt!= m_surfaces.end()) {
		if( (*itt).second.surface == surface) {
			--((*itt).second.refCount);
			if(! (*itt).second.refCount) {
				SDL_FreeSurface((*itt).second.surface);
				m_surfaces.erase(itt);
			}
			return;
			
		}
		++itt;
	}

	ERROR("Tried to free a surface, could not be found.");
}

SDL_Surface *SurfaceManager::add(std::string filename) {
	// FIXME: Dummy, we need the SDL_PixelFormat
	return 0;
}

SDL_Surface *SurfaceManager::add(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha)
{
	// Try and load any image format first. If that fails, try and force loading as TGA. If that fails,
	// Crash 'n burn :-)

	SDL_Surface *surf = 0;
	int data_dirs = sizeof(data_dir)/sizeof(std::string);
	for (int i = 0; i < data_dirs; i++) {
		std::string imgfile = std::string(data_dir[i]);
		imgfile.append("/");
		imgfile.append(filename.c_str());
		surf = IMG_Load(imgfile.c_str());
		if (surf) break;
	}

	if (!surf) {	
		SDL_RWops *rw = SDL_RWFromFile(filename.c_str(), "r");	
		
		if(rw==0) {
			ERROR("could not open image \"" << filename.c_str() << "\": " << SDL_GetError());
			return 0;
		}

		surf = IMG_LoadTGA_RW(rw);	
		if(surf == 0) {
			ERROR("could not load image : " << SDL_GetError());
			SDL_RWclose(rw);
			return 0;
		}
		SDL_RWclose(rw);
	}

	if(colorKey) {
		Uint32 key = getPixel(surf, 0, 0);
		if(SDL_SetColorKey(surf, SDL_SRCCOLORKEY | SDL_RLEACCEL, key) < 0) {
			ERROR("could not apply color key to surface : " << SDL_GetError());
		}
	}
	if(alpha) {
		if(SDL_SetAlpha(surf, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE) < 0) {
			ERROR("could not set Alpha to surface : " << SDL_GetError());
		}
	}

	if(colorKey || alpha) {
		m_surfaces[filename].surface = SDL_DisplayFormatAlpha(surf);
	} else {
		m_surfaces[filename].surface = SDL_DisplayFormat(surf);
	}
	if(!m_surfaces[filename].surface) {
		ERROR("could not convert surface to Display Format : " << SDL_GetError());
	}
	SDL_FreeSurface(surf);
	
	m_surfaces[filename].refCount = 1;
	
	return m_surfaces[filename].surface;
}

Uint32 SurfaceManager::getPixel(SDL_Surface *surface, int x, int y)
{
	if( SDL_LockSurface(surface) < 0) {
		ERROR("Cannot lock surface : " << SDL_GetError());
		return 0;
	}
	
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	Uint32 val;
	
	switch(bpp) {
		case 1:
			val = *p;
			break;
		case 2:
			val = *(Uint16 *)p;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				val = p[0] << 16 | p[1] << 8 | p[2];
			else
				val = p[0] | p[1] << 8 | p[2] << 16;
			break;
		case 4:
			val = *(Uint32 *)p;
			break;
		default:
			/* never happens, but avoids warnings */
			val = 0;
			break;
	}

	SDL_UnlockSurface(surface);

	return val;
}
