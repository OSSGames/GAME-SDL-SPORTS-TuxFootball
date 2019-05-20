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

#include "menuoptionlist.h"

#include <iostream>

#include "SFont.h"

MenuOptionList::MenuOptionList(Menu *menu, std::string name, std::string identifier) :
				MenuItem(menu, name, identifier)
{
	m_name = name;
	m_identifier = identifier;
	m_curOption = m_optionList.begin();
}

MenuOptionList::~MenuOptionList()
{
}

MenuOptionList::MenuOptionList(const MenuOptionList &item) :
				MenuItem(item)
{
	m_name = item.name();
	m_identifier = item.identifier();
	m_optionList = item.optionList();
	m_curOption = m_optionList.begin();
}

const MenuOptionList &MenuOptionList::operator=(const MenuOptionList &item)
{
	m_name = item.name();
	m_optionList = item.optionList();
	m_curOption = m_optionList.begin();
	return *this;
}

void MenuOptionList::addOption(std::string option, int value)
{
	MenuOption opt;
	opt.text = option;
	opt.value = value;
	m_optionList.push_back(opt);
	m_curOption = m_optionList.begin();
}

std::string MenuOptionList::name() const
{
	return m_name;
}

std::string MenuOptionList::currentOptionText() const
{
	if(isOption()) {
		return (*m_curOption).text;
	} else {
		return "";
	}
}

int MenuOptionList::currentOptionValue() const
{
	if(isOption()) {
		return (*m_curOption).value;
	} else {
		return -1;
	}
}

int MenuOptionList::width(SFont* font)
{
	return font->getTextWidth(text().c_str());
}

int MenuOptionList::maximumWidth(SFont* font)
{
	if(!isOption()) return width(font);

	int maxWidth = 0;

	std::list<MenuOption>::iterator itt;

	for(itt=m_optionList.begin(); itt!=m_optionList.end(); ++itt) {
		int testWidth = font->getTextWidth(calculateTextString((*itt).text).c_str());

		if(testWidth > maxWidth) {
			maxWidth = testWidth;
		}
	}

	return maxWidth;
}

std::string MenuOptionList::text() const
{
	if(isOption()) {
		return calculateTextString((*m_curOption).text);
	}

	return m_name;
}

void MenuOptionList::selectOption(std::string option)
{
	if(!isOption()) return;

	std::list<MenuOption>::iterator itt;

	for(itt=m_optionList.begin(); itt!=m_optionList.end(); ++itt) {
		if((*itt).text == option) {
			m_curOption = itt;
			return;
		}
	}
}

void MenuOptionList::selectOption(int value)
{
	if(!isOption()) return;

	std::list<MenuOption>::iterator itt;

	for(itt=m_optionList.begin(); itt!=m_optionList.end(); ++itt) {
		if((*itt).value == value) {
			m_curOption = itt;
			return;
		}
	}
}

void MenuOptionList::incrementOption()
{
	++m_curOption;

	if(m_curOption == m_optionList.end()) {
		m_curOption = m_optionList.begin();
	}
}

void MenuOptionList::decrementOption()
{
	if(m_curOption == m_optionList.begin()) {
		m_curOption = m_optionList.end();
	}
	--m_curOption;
}

std::string MenuOptionList::calculateTextString(std::string option) const
{
	return m_name + " " + option;
}

const std::list<MenuOption> &MenuOptionList::optionList() const
{
	return m_optionList;
}

bool MenuOptionList::isOption() const
{
	return (!m_optionList.empty());
}

std::string MenuOptionList::identifier() const
{
	return m_identifier;
}

void MenuOptionList::leftPressed()
{
	decrementOption();
}

void MenuOptionList::rightPressed()
{
	incrementOption();
}

void MenuOptionList::selectPressed()
{
}

void MenuOptionList::update(Uint8 *keys)
{
	// Does not react to keys
	(void) keys;
}

