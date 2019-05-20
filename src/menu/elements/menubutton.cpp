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

#include "menubutton.h"

#include <iostream>

#include "menu/menu.h"
#include "Font.hpp"

MenuButton::MenuButton(Menu *menu, std::string name, std::string identifier) :
				MenuItem(menu, name, identifier)
{
	m_name = name;
	m_identifier = identifier;
}

MenuButton::~MenuButton()
{
}

MenuButton::MenuButton(const MenuButton &item) :
			MenuItem(item)
{
	m_name = item.name();
	m_identifier = item.identifier();
}

const MenuButton &MenuButton::operator=(const MenuButton &item)
{
	m_name = item.name();
	return *this;
}

std::string MenuButton::name() const
{
	return m_name;
}

std::string MenuButton::currentOptionText() const
{
	return "";
}

int MenuButton::currentOptionValue() const
{
	return -1;
}

int MenuButton::width(SFont* font)
{
	return font->getTextWidth(text().c_str());
}

int MenuButton::maximumWidth(SFont* font)
{
	return width(font);
}

std::string MenuButton::text() const
{
	return m_name;
}

std::string MenuButton::calculateTextString(std::string option) const
{
	return m_name + " " + option;
}

std::string MenuButton::identifier() const
{
	return m_identifier;
}

void MenuButton::leftPressed()
{
}

void MenuButton::rightPressed()
{
}

void MenuButton::selectPressed()
{
	m_menu->setFinished();
}

void MenuButton::update(Uint8 *keys)
{
	// Does not react to keys
	(void) keys;
}
