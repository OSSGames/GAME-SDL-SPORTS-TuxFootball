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

#include "spriteobject.h"

#include <iostream>

#include "logger/logger.h"

SpriteObject::SpriteObject(SDL_Surface *surf, SDL_Surface *shadow, SDL_Surface *overlay,
				Point3D position, SDL_Rect offset, SDL_Rect shadowOffset, SDL_Rect overlayOffset)
{
	m_surface = surf;
	m_shadow = shadow;
	m_overlay = overlay;

	m_position = position;
	m_offset = offset;
	m_shadowOffset = shadowOffset;
	m_overlayOffset = overlayOffset;
}

SpriteObject::~SpriteObject()
{
}

void SpriteObject::draw(int left, int top, SDL_Surface *surface)
{
	SDL_Rect r, s;

	if(surface==0) return;
	if(m_surface==0) return;

	r.x = 0;
	r.y = 0;
	r.w = m_surface->w;
	r.h = m_surface->h;

	s.x = (int)(m_position.x() - left + m_offset.x);
	s.y = (int)(m_position.y() - top + m_offset.y - m_position.z());

	if(SDL_BlitSurface(m_surface, &r, surface, &s) < 0) {
		ERROR("could not draw SpriteObject to screen : " << SDL_GetError());
	}

}

void SpriteObject::drawShadow(int left, int top, SDL_Surface *surface)
{
	SDL_Rect r, s;

	if(surface==0) return;
	if(m_shadow==0) return;

	r.x = 0;
	r.y = 0;
	r.w = m_shadow->w;
	r.h = m_shadow->h;

	s.x = (int)(m_position.x() - left + m_shadowOffset.x + m_position.z());
	s.y = (int)(m_position.y() - top + m_shadowOffset.y - (m_position.z()*0.8));

	if(SDL_BlitSurface(m_shadow, &r, surface, &s) < 0) {
		ERROR("could not blit SpriteObject shadow to screen : " << SDL_GetError());
	}
}

void SpriteObject::drawOverlay(int left, int top, SDL_Surface *surface)
{
	SDL_Rect r, s;

	if(surface==0) return;
	if(m_overlay==0) return;

	r.x = 0;
	r.y = 0;
	r.w = m_overlay->w;
	r.h = m_overlay->h;

	s.x = (int)(m_position.x() - left + m_overlayOffset.x);
	s.y = (int)(m_position.y() - top + m_overlayOffset.y - m_position.z());

	// if overlay is off-screen, then draw it at the closest point on-screen that there is.
	if(s.x<0) s.x = 0;
	if(s.x + r.w > surface->w) s.x = surface->w - r.w;
	if(s.y<0) s.y = 0;
	if(s.y + r.h > surface->h) s.y = surface->h - r.h;

	if(SDL_BlitSurface(m_overlay, &r, surface, &s) < 0) {
		ERROR("could not blit SpriteObject overlay to screen : " << SDL_GetError());
	}
}

void SpriteObject::setSurface(SDL_Surface *surf)
{
	m_surface = surf;
}

void SpriteObject::setOverlaySurface(SDL_Surface *surf)
{
	m_overlay = surf;
}

void SpriteObject::setShadowSurface(SDL_Surface *surf)
{
	m_shadow = surf;
}

void SpriteObject::setPosition(const Point3D &pos)
{
	m_position = pos;
}

Point3D SpriteObject::position()
{
	return m_position;
}
