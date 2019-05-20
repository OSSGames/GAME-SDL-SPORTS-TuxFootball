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

#include "menukeyselect.h"

#include <iostream>

#include "menu/menu.h"
#include "SFont.h"

MenuKeySelect::MenuKeySelect(Menu *menu, SDLKey key, std::string name, std::string identifier) :
				MenuItem(menu, name, identifier)
{
	m_key = key;
	m_name = name;
	m_identifier = identifier;
}

MenuKeySelect::~MenuKeySelect()
{
}

MenuKeySelect::MenuKeySelect(const MenuKeySelect &item) :
				MenuItem(item)
{
	m_name = item.name();
	m_identifier = item.identifier();
}

const MenuKeySelect &MenuKeySelect::operator=(const MenuKeySelect &item)
{
	m_name = item.name();
	return *this;
}

std::string MenuKeySelect::name() const
{
	return m_name;
}

std::string MenuKeySelect::currentOptionText() const
{
	return "";
}

int MenuKeySelect::currentOptionValue() const
{
	return -1;
}

int MenuKeySelect::width(SFont* font)
{
	return font->getTextWidth(text().c_str());
}

int MenuKeySelect::maximumWidth(SFont* font)
{
	return width(font);
}

std::string MenuKeySelect::text() const
{
	return m_name + " " + SDL_GetKeyName(m_key);
}

std::string MenuKeySelect::identifier() const
{
	return m_identifier;
}

void MenuKeySelect::leftPressed() 
{
}

void MenuKeySelect::rightPressed()
{
}

void MenuKeySelect::selectPressed()
{
	for(int count=0; count<SDLK_LAST; count++) m_keystate[count] = 0;
	
	m_menu->grabFocus(this);
}

void MenuKeySelect::update(Uint8 *keys)
{
	for(int count=0; count<SDLK_LAST; count++) {
		if((m_keystate[count]) && (!keys[count])) {
			m_menu->grabFocus(0);
			m_key = (SDLKey)count;
		}
		m_keystate[count] = keys[count];
	}
}

SDLKey MenuKeySelect::currentKey()
{
	return m_key;
}
