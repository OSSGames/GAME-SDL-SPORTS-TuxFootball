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

#ifndef TUXFOOTBALL_MENUITEM
#define TUXFOOTBALL_MENUITEM

#include <string>
#include <list>

#include "SDL.h"
#include "Font.hpp"

class Menu;

/* A menu item, a base class for any widget that exists in a menu. */
class MenuItem {
public:
	MenuItem(Menu *menu, std::string name, std::string identifier);
	virtual ~MenuItem();
	MenuItem(const MenuItem &item);
	const MenuItem &operator=(const MenuItem &item);
	std::string name() const;
	std::string identifier() const;
	void incrementReference();
	void decrementReference();

	virtual std::string text() const = 0;
	virtual std::string currentOptionText() const = 0;
	virtual int currentOptionValue() const = 0;
	virtual int width(SFont* font) = 0;
	virtual int height();
	virtual int maximumWidth(SFont* font) = 0;
	virtual void leftPressed() = 0;
	virtual void rightPressed() = 0;
	virtual void selectPressed() = 0;
	virtual void update(Uint8 *keys) = 0;
	virtual void draw(SFont* font, int x, int y, int wid, SDL_Surface *screen);
private: // methods
	std::string calculateTextString(std::string option) const;
private: // attributes
	std::string m_name;
	std::string m_identifier;
	int m_refCount;
protected:
	Menu *m_menu;
};

#endif /* TUXFOOTBALL_MENUITEM */
