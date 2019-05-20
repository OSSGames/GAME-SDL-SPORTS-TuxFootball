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

#ifndef TUXFOOTBALL_MENU
#define TUXFOOTBALL_MENU

#include <string>
#include <list>

#include "elements/menuitem.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include "Font.hpp"

class MenuItem;

class MenuItemContainer {
public:
	MenuItemContainer(MenuItem *item) {
		m_menuItem = item;
		m_menuItem->incrementReference();
	}
	
	MenuItemContainer(const MenuItemContainer &cont) {
		m_menuItem = (*cont);
		m_menuItem->incrementReference();
	}

	MenuItemContainer &operator=(const MenuItemContainer &cont) {
		if(m_menuItem) {
			m_menuItem->decrementReference();
			m_menuItem = 0;
		}
		m_menuItem = (*cont);
		m_menuItem->incrementReference();
		return *this;
	}
	
	MenuItem *operator*() const {
		return m_menuItem;
	}
	
	~MenuItemContainer() {
		m_menuItem->decrementReference();
	}	
private:
	MenuItem *m_menuItem;
};

/** Baseclass for menu functionality. */
class Menu
{
public:
	enum Position {Centered};
	Menu(SDL_Surface *screen, std::string name);
	virtual ~Menu();
	void setPosition(Position pos, int x, int y);
	void addItem(MenuItem *item);
	void show();
	void hide();
	void draw();
	void update(Uint8 *keys);
	void calculateMenuBounds();
	bool cancelled();
	bool finished();
	bool selectionMade();
	const MenuItem *selected();
	const MenuItem *menuItem(std::string name);
	void setFinished();
	void setCancelled();
	void grabFocus(MenuItem *item);
private:
	bool m_finished;
	bool m_selectionMade;
	bool m_cancelled;

	SDL_Rect m_menuBounds;
	std::string m_name;
	bool m_show;
	std::list<MenuItemContainer> m_itemList;

	Position m_pos;
	int m_posx;
	int m_posy;

	std::list<MenuItemContainer>::iterator m_active;

	int m_menuPadding_w;
	int m_menuPadding_h;

	SFont *m_activeFont;
	SFont *m_inactiveFont;

	SDL_Surface *m_screen;
	SDL_Surface *m_logo;
	SDL_Surface *m_background;

	Mix_Chunk *m_beep;
	Mix_Chunk *m_incrementBeep;
	Mix_Chunk *m_decrementBeep;
	Mix_Chunk *m_selectBeep;

	MenuItem *m_grabFocus;
};

#endif /* TUXFOOTBALL_MENU */
