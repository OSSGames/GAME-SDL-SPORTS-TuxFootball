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

#ifndef TUXFOOTBALL_SURFACEMANAGER
#define TUXFOOTBALL_SURFACEMANAGER

#include "resourcemanager.h"

#include <SDL.h>
#include <string>
#include <map>


struct ManagedSurface {
	SDL_Surface *surface;
	int refCount;
};

/** Manages surfaces, makes sure that they are only loaded once and are deleted
 * when no longer needed. */
class SurfaceManager : public ResourceManager<SDL_Surface, SurfaceManager> {
public:
	virtual SDL_Surface *load(std::string filename);

	/** If colorKey is true, then the image will use the top-leftmost pixel as the color key. */
	virtual SDL_Surface *load(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha);
	virtual void release(SDL_Surface *surface);
protected:
	virtual SDL_Surface *add(SDL_PixelFormat *format, std::string filename, bool colorKey, bool alpha);
	virtual SDL_Surface *add(std::string filename);
private:
	static std::map<std::string, ManagedSurface> m_surfaces;
	static Uint32 getPixel(SDL_Surface *surface, int x, int y);
};

#endif /* TUXFOOTBALL_SURFACEMANAGER */
