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

#ifndef TUXFOOTBALL_MENUOPTIONLIST
#define TUXFOOTBALL_MENUOPTIONLIST

#include "menuitem.h"

#include <string>
#include <list>

class Menu;

struct MenuOption {
	std::string text;
	int value;
};

/** Specifies a menu item with multiple selectable choices. */
class MenuOptionList : public MenuItem {
public:
	MenuOptionList(Menu *menu, std::string name, std::string identifier);
	~MenuOptionList();
	MenuOptionList(const MenuOptionList &item);
	const MenuOptionList &operator=(const MenuOptionList &item);
	std::string name() const;
	void addOption(std::string option, int value);
	std::string currentOptionText() const;
	int currentOptionValue() const;
	int width(SFont* font);
	int maximumWidth(SFont* font);
	std::string text() const;
	void selectOption(std::string option);
	void selectOption(int value);	
	void incrementOption();
	void decrementOption();
	bool isOption() const;
	const std::list<MenuOption> &optionList() const;
	std::string identifier() const;
	void leftPressed();
	void rightPressed();
	void selectPressed();
	void update(Uint8 *keys);
private:
	std::list<MenuOption> m_optionList;
	std::list<MenuOption>::iterator m_curOption;
	std::string m_name;
	std::string m_identifier;

	std::string calculateTextString(std::string option) const;
};

#endif /* TUXFOOTBALL_MENUOPTIONLIST */
