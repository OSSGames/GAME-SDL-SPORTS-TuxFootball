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

#include <iostream>
#ifdef HAVE_GETTEXT
#include "../gettext.h"
#define _(String) gettext (String)
#else
#define _(String) (String)
#endif

#include "redefinekeysstate.h"

#include "menu/redefinekeysmenu.h"
#include "gameengine.h"

RedefineKeysState::RedefineKeysState(GameEngine &engine) :
			MenuStateBase(engine)
{
}

RedefineKeysState::~RedefineKeysState()
{
}

void RedefineKeysState::enterState()
{
	MenuStateBase::enterState();

	m_engine.setupHalfTime();
}

void RedefineKeysState::leaveState()
{
}

void RedefineKeysState::updateLoop()
{
	Menu* menu = MenuStateBase::menu();
	if(menu) {
		if(menu->selectionMade()) {
			m_engine.setState(GameEngine::TitleScreen);
		}
	} else {
		m_engine.setState(GameEngine::TitleScreen);
	}
}

void RedefineKeysState::initialiseMenu()
{
	
	Menu *menu = new RedefineKeysMenu(m_engine.controller(GameEngine::HomeController),
					 m_engine.controller(GameEngine::AwayController),
					 m_engine.screen(), _("Redefine Keys"));
	menu->show();
	setMenu(menu);
}
