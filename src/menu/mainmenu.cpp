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

#include "mainmenu.h"

#include <iostream>
#ifdef HAVE_GETTEXT
#include "../gettext.h"
#define _(String) gettext (String)
#else
#define _(String) (String)
#endif

#include "elements/menulabel.h"
#include "elements/menubutton.h"
#include "elements/menuoptionlist.h"
#include "controller.h"
#include "gameengine.h"
#include "logger/logger.h"

MainMenu::MainMenu(GameEngine *gameEngine, Controller *home, Controller *away, SDL_Surface *screen, std::string name) :
				Menu(screen, name),
				m_gameEngine(gameEngine),
				m_home(home),
				m_away(away)
{
	addItem(new MenuButton(this, _("Play Game!"), "start"));
	MenuOptionList *length = new MenuOptionList(this, _("Half Length :"), "halflength");
	length->addOption(_("1 minute"), 6000);
	length->addOption(_("2 minutes"), 12000);
	length->addOption(_("5 minutes"), 30000);
	length->addOption(_("10 minutes"), 60000);
	length->addOption(_("20 minutes"), 120000);
	length->addOption(_("45 minutes"), 270000);
	addItem(length);
	MenuOptionList *who = new MenuOptionList(this, _("Players :"), "players");
	who->addOption(_("Player V Computer"), 1);
	who->addOption(_("Player V Player"), 3);
	who->addOption(_("Computer V Computer"), 0);
	addItem(who);
	addItem(new MenuButton(this, _("Video Settings"), "video"));
	addItem(new MenuButton(this, _("Redefine Keys"), "redefine"));
	addItem(new MenuButton(this, _("Exit"), "exit"));
}

MainMenu::~MainMenu()
{
	int halfLength;
	const MenuItem *item = menuItem("halflength");
	if(item) {
		halfLength = item->currentOptionValue();
	} else {
		halfLength = 100;
	}
	m_gameEngine->setHalfLength(halfLength);

	item = menuItem("players");
	if(item) {
		int value = item->currentOptionValue();
		m_home->setCPU( (value & 1) == 0);
		m_away->setCPU( (value & 2) == 0);
	} else {
		ERROR("number of players not defined");
		m_home->setCPU(true);
		m_away->setCPU(false);
	}
}
