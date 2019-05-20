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

#include "graphics.h"

#include <iostream>

#include "logger/logger.h"

Graphics::Graphics(SDL_Surface *screen)
{
	m_screenSurface = screen;
}

Graphics::~Graphics()
{
}

void Graphics::addSprite(SpriteObject *obj)
{
	m_sprites.push_back(SpriteContainer(obj));
}

void Graphics::removeSprite(SpriteObject *obj)
{
	std::list<SpriteContainer>::iterator itt;

	for(itt = m_sprites.begin(); itt!=m_sprites.end(); ++itt) {
		if((*(*itt))==obj) {
			m_sprites.erase(itt);
			return;
		}
	}
	WARN("Graphics::removeSprite() : sprite does not exist");
}

void Graphics::draw(int left, int top)
{
	std::list<SpriteContainer>::iterator itt;

	for(itt=m_sprites.begin(); itt!=m_sprites.end(); ++itt) {
		(*(*itt))->drawShadow(left, top, m_screenSurface);
	}
	for(itt=m_sprites.begin(); itt!=m_sprites.end(); ++itt) {
		(*(*itt))->draw(left, top, m_screenSurface);
	}
	for(itt=m_sprites.begin(); itt!=m_sprites.end(); ++itt) {
		(*(*itt))->drawOverlay(left, top, m_screenSurface);
	}
}

void Graphics::update()
{
	m_sprites.sort();
}


SDL_Surface *Graphics::screen() {
	return m_screenSurface;
}
