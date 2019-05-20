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

#ifndef TUXFOOTBALL_GRAPHICS
#define TUXFOOTBALL_GRAPHICS

#include "SDL.h"
#include "spriteobject.h"
#include <list>

/** Handles the correct drawing of sprites on screen. */
class SpriteContainer {
public:
	SpriteContainer(SpriteObject *obj) {
		m_so = obj;
	}
	
	~SpriteContainer() {
	}
	
	SpriteObject *operator*() {
		return m_so;
	}

	bool operator<(SpriteContainer &cont) {
		return (m_so->position().y() < (*cont)->position().y());
	}
private:
	SpriteObject *m_so;
};

class Graphics {
public:
	Graphics(SDL_Surface *screen);
	~Graphics();
	void addSprite(SpriteObject *obj);
	void removeSprite(SpriteObject *obj);
	void draw(int left, int top);
	void update();
	SDL_Surface *screen();
private:
	std::list<SpriteContainer> m_sprites;
	SDL_Surface *m_screenSurface;
};

#endif
