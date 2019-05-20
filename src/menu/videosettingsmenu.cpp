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

#include "videosettingsmenu.h"

#include "elements/menubutton.h"
#include "elements/menuoptionlist.h"

VideoSettingsMenu::VideoSettingsMenu(bool currentMode, SDL_Surface *screen, std::string name) :
							Menu(screen, name),
							m_fullscreen(currentMode)
{
	addItem(m_modes = new MenuOptionList(this, "Video mode", "videomode"));
	m_modes->addOption("Fullscreen", true);
	m_modes->addOption("Windowed", false);
	m_modes->selectOption(currentMode);
	addItem(new MenuButton(this, "Exit", "exit"));
}

VideoSettingsMenu::~VideoSettingsMenu()
{
}

bool
VideoSettingsMenu::currentMode() const
{
	return m_modes->currentOptionValue() == 1;
}
