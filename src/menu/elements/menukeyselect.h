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

#ifndef TUXFOOTBALL_MENUKEYSELECT
#define TUXFOOTBALL_MENUKEYSELECT

#include "menuitem.h"

#include <string>
#include <list>
#include <SDL.h>

class Menu;

/** A menu item that remembers a key press and allows it to be redefined. */
class MenuKeySelect : public MenuItem {
public:
	MenuKeySelect(Menu *menu, SDLKey key, std::string name, std::string identifier);
	~MenuKeySelect();
	MenuKeySelect(const MenuKeySelect &item);
	const MenuKeySelect &operator=(const MenuKeySelect &item);
	std::string name() const;
	std::string currentOptionText() const;
	int currentOptionValue() const;
	int width(SFont* font);
	int maximumWidth(SFont* font);
	std::string text() const;
	void incrementOption();
	void decrementOption();
	std::string identifier() const;
	void leftPressed();
	void rightPressed();
	void selectPressed();
	void update(Uint8 *keys);
	SDLKey currentKey();
private:
	Uint8 m_keystate[SDLK_LAST];
	std::string m_name;
	std::string m_identifier;
	SDLKey m_key;
};

#endif /* TUXFOOTBALL_MENUKEYSELECT */
