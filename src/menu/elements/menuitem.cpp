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

#include "menuitem.h"

#include <iostream>

#include "SFont.h"

MenuItem::MenuItem(Menu *menu, std::string name, std::string identifier)
{
	m_menu = menu;
	m_name = name;
	m_identifier = identifier;
	m_refCount = 0;
}

MenuItem::~MenuItem()
{
}

MenuItem::MenuItem(const MenuItem &item)
{
	m_name = item.name();
	m_identifier = item.identifier();
}

const MenuItem &MenuItem::operator=(const MenuItem &item)
{
	m_name = item.name();
	return *this;
}

std::string MenuItem::name() const
{
	return m_name;
}

std::string MenuItem::calculateTextString(std::string option) const
{
	return m_name + " " + option;
}

std::string MenuItem::identifier() const
{
	return m_identifier;
}

void MenuItem::incrementReference()
{
	m_refCount++;
}

void MenuItem::decrementReference()
{
	m_refCount--;
	if(!m_refCount) delete this;
}

int MenuItem::height() 
{
	return 30;
}

void MenuItem::draw(SFont* font, int x, int y, int wid, SDL_Surface *screen) 
{
	int midX = x + (wid/2);
	
	font->write(screen, text().c_str(), midX - (width(font)/2), y);
}
