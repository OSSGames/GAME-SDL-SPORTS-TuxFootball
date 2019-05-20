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

#include "menu.h"

#include <iostream>

#include "resources/fontmanager.h"
#include "resources/surfacemanager.h"
#include "resources/soundmanager.h"
#include "Font.hpp"
#include "logger/logger.h"

Menu::Menu(SDL_Surface *screen, std::string name)
{
	m_name = name;
	m_show = false;
	m_screen = screen;

	m_finished = false;
	m_selectionMade = false;
	m_cancelled = false;

	m_pos = Centered;
	m_menuBounds.x = m_screen->w/2;
	m_menuBounds.y = m_screen->h/2;
	m_menuBounds.w = 0;
	m_menuBounds.h = 0;

	m_menuPadding_w = 15;
	m_menuPadding_h = 15;

	// FIXME: GCC 4 issue m_active = 0;

	std::string str = "graphics/font_white.png";
	m_activeFont = FontManager::instance()->load(m_screen->format, str, false, true);
	str = "graphics/font_yellow.png";
	m_inactiveFont = FontManager::instance()->load(m_screen->format, str, false, true);

	m_logo = SurfaceManager::instance()->load(screen->format, "graphics/tuxfootball.png", false, true);
	m_background = SurfaceManager::instance()->load(screen->format, "graphics/menu_background.png", false, true);

	m_beep = SoundManager::instance()->load("sound/beep.wav");
	m_incrementBeep = SoundManager::instance()->load("sound/increment_beep.wav");
	m_decrementBeep = SoundManager::instance()->load("sound/decrement_beep.wav");
	m_selectBeep = SoundManager::instance()->load("sound/select_beep.wav");

	m_grabFocus = 0;
}

Menu::~Menu()
{
	if(m_logo) SurfaceManager::instance()->release(m_logo);
	if(m_background) SurfaceManager::instance()->release(m_background);
	if(m_activeFont) FontManager::instance()->release(m_activeFont);
	if(m_inactiveFont) FontManager::instance()->release(m_inactiveFont);
	if(m_beep) SoundManager::instance()->release(m_beep);
	if(m_incrementBeep) SoundManager::instance()->release(m_incrementBeep);
	if(m_decrementBeep) SoundManager::instance()->release(m_decrementBeep);
	if(m_selectBeep) SoundManager::instance()->release(m_selectBeep);
}

void Menu::setPosition(Position pos, int x, int y)
{
	m_pos = pos;
	m_posx = x;
	m_posy = y;
	if(m_show) {
		calculateMenuBounds();
	}
}

void Menu::addItem(MenuItem *item)
{
	m_itemList.push_back(MenuItemContainer(item));

	// FIXME: GCC 4 issue if(m_active==0)
	m_active = m_itemList.begin();

	if(m_show) {
		calculateMenuBounds();
	}
}

void Menu::show()
{
	calculateMenuBounds();
	m_show = true;
}

void Menu::hide()
{
	m_show = false;
}

void Menu::update(Uint8 *keys)
{
	static bool upPressed = false;
	static bool downPressed = false;
	static bool leftPressed = false;
	static bool rightPressed = false;
	static bool escapePressed = false;
	static bool returnPressed = false;

	if(m_finished) return;

	if(m_grabFocus) {
		m_grabFocus->update(keys);
	} else {
		if(keys[SDLK_ESCAPE]) {
			if(!escapePressed) {
				if(m_selectBeep) Mix_PlayChannel(-1, m_selectBeep, 0);
				escapePressed = true;
			}
		} else if(escapePressed) {
			escapePressed = false;
		}

		if(keys[SDLK_RETURN]) {
			if(!returnPressed) {
				returnPressed = true;
				if(m_selectBeep) Mix_PlayChannel(-1, m_selectBeep, 0);
			}
		} else if(returnPressed) {
			(*(*m_active))->selectPressed();
			returnPressed = false;
		}

		if(keys[SDLK_UP]) {
			if(!upPressed) {
				upPressed = true;
				if(m_beep) Mix_PlayChannel(-1, m_beep, 0);
				if(m_active == m_itemList.begin()) {
					m_active = m_itemList.end();
				}
				--m_active;
			}
		} else {
			upPressed = false;
		}

		if(keys[SDLK_DOWN]) {
			if(!downPressed) {
				downPressed = true;
				if(m_beep) Mix_PlayChannel(-1, m_beep, 0);
				++m_active;
				if(m_active==m_itemList.end()) {
					m_active = m_itemList.begin();
				}
			}
		} else {
			downPressed = false;
		}

		if(keys[SDLK_LEFT]) {
			if(!leftPressed) {
				leftPressed = true;
				if(m_decrementBeep) Mix_PlayChannel(-1, m_decrementBeep, 0);
				(*(*m_active))->leftPressed();
			}
		} else {
			leftPressed = false;
		}

		if(keys[SDLK_RIGHT]) {
			if(!rightPressed) {
				rightPressed = true;
				if(m_incrementBeep) Mix_PlayChannel(-1, m_incrementBeep, 0);
				(*(*m_active))->rightPressed();
			}
		} else {
			rightPressed = false;
		}
	}
}

void Menu::draw()
{
	SDL_Rect r, s;
	std::list<MenuItemContainer>::iterator itt;

	if(!m_show) return;

	for(int y=0; y<m_menuBounds.h; y+=m_background->h) {
		for(int x=0; x<m_menuBounds.w; x+=m_background->w) {
			r.x = 0;
			r.y = 0;
			r.w = (m_menuBounds.w - x < m_background->w) ? m_menuBounds.w - x : m_background->w;
			r.h = (m_menuBounds.h - y < m_background->h) ? m_menuBounds.h - y : m_background->h;
			s.x = x + m_menuBounds.x;
			s.y = y + m_menuBounds.y;

			if(SDL_BlitSurface(m_background, &r, m_screen, &s) < 0) {
				ERROR("could not pitch tile : " << SDL_GetError());
			}
		}
	}

	int curY = m_menuBounds.y + m_menuPadding_h;

	for(itt = m_itemList.begin(); itt!=m_itemList.end(); ++itt) {
		if(itt == m_active) {
			(*(*itt))->draw(m_activeFont, m_menuBounds.x, curY, m_menuBounds.w, m_screen);
		} else {
			(*(*itt))->draw(m_inactiveFont, m_menuBounds.x, curY, m_menuBounds.w, m_screen);
		}
		curY += (*(*itt))->height();
	}
}

void Menu::calculateMenuBounds()
{
	std::list<MenuItemContainer>::iterator itt;

	m_menuBounds.w = 0;
	m_menuBounds.h = m_menuPadding_h*2;

	for(itt = m_itemList.begin(); itt!=m_itemList.end(); ++itt) {
		if(m_menuBounds.w < (*(*itt))->maximumWidth(m_activeFont)) {
			m_menuBounds.w = (*(*itt))->maximumWidth(m_activeFont);
		}
		m_menuBounds.h += (*(*itt))->height();
	}
	m_menuBounds.w += m_menuPadding_w*2;

	switch(m_pos) {
		case Centered :	m_menuBounds.x = (m_screen->w - m_menuBounds.w)/2;
				m_menuBounds.y = (m_screen->h - m_menuBounds.h)/2;
			break;
		default:
			break;
	}
}

bool Menu::cancelled()
{
	return m_cancelled;
}

bool Menu::selectionMade()
{
	return m_selectionMade;
}

bool Menu::finished()
{
	return m_finished;
}

const MenuItem *Menu::selected()
{
	return *(*m_active);
}

const MenuItem *Menu::menuItem(std::string name)
{
	std::list<MenuItemContainer>::iterator itt;

	for(itt = m_itemList.begin(); itt!=m_itemList.end(); ++itt) {
		DEBUG("checking " << (*(*itt))->identifier() << " against " << name);
		if((*(*itt))->identifier().compare(name)==0) {
			return (*(*itt));
		}
	}

	WARN("menuItem not found");
	return 0;
}

void Menu::setFinished()
{
	m_finished=true;
	m_selectionMade = true;
}

void Menu::grabFocus(MenuItem *item)
{
	m_grabFocus = item;
}

void Menu::setCancelled()
{
	m_cancelled = true;
	m_finished = true;
	m_selectionMade = true;
}
