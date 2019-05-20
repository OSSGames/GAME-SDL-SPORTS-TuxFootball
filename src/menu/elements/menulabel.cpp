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

#include "menulabel.h"

#include <iostream>

#include "menu.h"

MenuLabel::MenuLabel(Menu *menu, std::string name, std::string identifier) :
				MenuItem(menu, name, identifier)
{
	m_name = name;
	m_identifier = identifier;
}

MenuLabel::~MenuLabel()
{
}

MenuLabel::MenuLabel(const MenuLabel &item) :
			MenuItem(item)
{
	m_name = item.name();
	m_identifier = item.identifier();
}

const MenuLabel &MenuLabel::operator=(const MenuLabel &item)
{
	m_name = item.name();
	return *this;
}

std::string MenuLabel::name() const
{
	return m_name;
}

std::string MenuLabel::currentOptionText() const
{
	return "";
}

int MenuLabel::currentOptionValue() const
{
	return -1;
}

int MenuLabel::width(SFont* font)
{
	return font->getTextWidth(text().c_str());
}

int MenuLabel::maximumWidth(SFont* font)
{
	return width(font);
}

std::string MenuLabel::text() const
{
	return m_name;
}

std::string MenuLabel::calculateTextString(std::string option) const
{
	return m_name + " " + option;
}

std::string MenuLabel::identifier() const
{
	return m_identifier;
}

void MenuLabel::leftPressed()
{
}

void MenuLabel::rightPressed()
{
}

void MenuLabel::selectPressed()
{
}

void MenuLabel::update(Uint8 *keys)
{
	// Does not react to keys
	(void) keys;
}

int MenuLabel::height()
{
	return 40;
}

void MenuLabel::draw(SFont* font, int x, int y, int wid, SDL_Surface *screen)
{
	int midX = x + (wid/2);

	font->write(screen, text().c_str(), midX - (width(font)/2), y+10);
}
