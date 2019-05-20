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

#include "mainmenustate.h"

#include "gameengine.h"
#include "menu/mainmenu.h"
#include "logger/logger.h"

MainMenuState::MainMenuState(GameEngine &engine) :
			MenuStateBase(engine)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::enterState()
{
	MenuStateBase::enterState();

	m_engine.setCameraFollows(GameEngine::CameraPattern);
	m_engine.setupHalfTime();
	m_engine.setMusic("sound/titlemusic.ogg");
}

void MainMenuState::leaveState()
{
}

void MainMenuState::updateLoop()
{
	Menu* menu = MenuStateBase::menu();
	if(menu) {
		if(menu->selectionMade()) {
			if(menu->cancelled() || (menu->selected()->text() == _("Exit"))) {
				DEBUG("menu cancelled");
				m_engine.setFinished(true);
			} else if(menu->selected()->text() == _("Play Game!")) {
				m_engine.setState(GameEngine::MatchStart);
			} else if(menu->selected()->text() == _("Redefine Keys")) {
				m_engine.setState(GameEngine::RedefineKeysScreen);
			} else if(menu->selected()->text() == _("Video Settings")) {
				m_engine.setState(GameEngine::VideoSettingsMenu);
			} else {
				WARN("unknown menu selection " << menu->selected()->text());
				m_engine.setFinished(true);
			}
		}
	} else {
		ERROR("menu non-existant, quitting");
		m_engine.setFinished(true);
	}
	if((menu) && (menu->finished())) {
		clearLogo();
		setMenu(0);
	}
}

void MainMenuState::initialiseMenu()
{
	Menu *menu = new MainMenu(&m_engine, m_engine.controller(GameEngine::HomeController),
					m_engine.controller(GameEngine::AwayController),
					m_engine.screen(), "TitleScreen");
	menu->show();
	setMenu(menu);
}
