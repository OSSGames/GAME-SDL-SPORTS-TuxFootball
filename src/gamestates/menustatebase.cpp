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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "menustatebase.h"

#include <iostream>
#include <SDL.h>

#include "gameengine.h"
#include "resources/surfacemanager.h"
#include "menu/menu.h"
#include "logger/logger.h"

MenuStateBase::MenuStateBase(GameEngine &engine) :
				m_engine(engine), m_logo(0), m_menu(0)
{
}

MenuStateBase::~MenuStateBase()
{
	clearLogo();
	setMenu(0);
}

bool MenuStateBase::isGameInProgress() const
{
	return false;
}

void MenuStateBase::renderFrame()
{
	SDL_Rect r, s;

	//
	// Draw Menu
	//
	if(m_menu) m_menu->draw();

	if(m_logo) {
		s.x = (m_engine.screen()->w - m_logo->w) / 2;
		s.y = 50;
		r.x = 0;
		r.y = 0;
		s.w = r.w = m_logo->w;
		s.h = r.h = m_logo->h;


		if(SDL_BlitSurface(m_logo, &r, m_engine.screen(), &s) < 0) {
			ERROR("could not pitch tile : " << SDL_GetError());
		}
	}
}

void MenuStateBase::enterState()
{
	initialiseMenu();
	m_logo = SurfaceManager::instance()->load(m_engine.screen()->format, "graphics/tuxfootball.png", false, true);
}

void MenuStateBase::clearLogo()
{
	if(m_logo) {
		SurfaceManager::instance()->release(m_logo);
		m_logo = 0;
	}
}

void MenuStateBase::setMenu(Menu *menu)
{
	if(m_menu) {
		delete m_menu;
		m_menu = 0;
	}
	m_menu = menu;
}

Menu* MenuStateBase::menu()
{
	return m_menu;
}

void MenuStateBase::update(Uint8* keys)
{
	if(m_menu) {
		m_menu->update(keys);
	}
}
