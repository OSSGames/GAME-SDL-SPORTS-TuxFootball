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

#ifndef TUXFOOTBALL_MENULABEL
#define TUXFOOTBALL_MENULABEL

#include "menuitem.h"

#include <string>
#include <list>

class Menu;

/** An un-clickable menu item that displays a text label. */
class MenuLabel : public MenuItem {
public:
	MenuLabel(Menu *menu, std::string name, std::string identifier);
	~MenuLabel();
	MenuLabel(const MenuLabel &item);
	const MenuLabel &operator=(const MenuLabel &item);
	std::string name() const;
	int currentOptionValue() const;
	std::string currentOptionText() const;
	int width(SFont* font);
	int height();
	int maximumWidth(SFont* font);
	std::string text() const;
	bool isOption() const;
	std::string identifier() const;
	void leftPressed();
	void rightPressed();
	void selectPressed();
	void update(Uint8 *keys);
	void draw(SFont* font, int x, int y, int wid, SDL_Surface *screen);
private:
	std::string m_name;
	std::string m_identifier;

	std::string calculateTextString(std::string option) const;
};

#endif /* TUXFOOTBALL_MENULABEL */
