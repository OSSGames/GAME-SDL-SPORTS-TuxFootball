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

#ifndef TUXFOOTBALL_SPRITEOBJECT
#define TUXFOOTBALL_SPRITEOBJECT

#include "SDL.h"
#include "point3d.h"
#include "body.h"

/** Encapsulate a sprite object, which can consist of a shadow, normal surface
 * and an overlay. */
class SpriteObject {
public:
	/**
	 * Creates a new sprite.
	 * \param surf the surface containing sprite itself
	 * \param shadow the surface containing the sprites shadow
	 * \param overlay the surface containing an overlay for the sprite
	 * \param position the sprites position in the game
	 * \param offset the offset used to draw the sprite to the game surface (only x and y will be used)
	 * \param shadowOffset the offset used to draw the shadow of the sprite to the game surface (only x and y will be used)
	 * \param overlayOffset the offset used to draw the sprites overlay to the game surface (only x and y will be used)
	 */
	SpriteObject(SDL_Surface *surf, SDL_Surface *shadow, SDL_Surface *overlay,
				Point3D position, SDL_Rect offset, SDL_Rect shadowOffset, SDL_Rect overlayOffset);
	~SpriteObject();

	void setPosition(const Point3D &pos);
	Point3D position();

	void setSurface(SDL_Surface *surf);
	void setOverlaySurface(SDL_Surface *surf);
	void setShadowSurface(SDL_Surface *surf);

	void draw(int left, int top, SDL_Surface *surface);
	void drawShadow(int left, int top, SDL_Surface *surface);
	void drawOverlay(int left, int top, SDL_Surface *surface);

private:
	bool m_toDraw;
	SDL_Surface *m_surface;
	SDL_Surface *m_shadow;
	SDL_Surface *m_overlay;
	Point3D m_position;
	SDL_Rect m_offset;
	SDL_Rect m_shadowOffset;
	SDL_Rect m_overlayOffset;
};

#endif
