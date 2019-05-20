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
#include "config.h"
#endif

#ifdef HAVE_GETTEXT
#include "../gettext.h"
#define _(String) gettext (String)
#else
#define _(String) (String)
#endif

#include "redefinekeysmenu.h"
#include "elements/menulabel.h"
#include "elements/menubutton.h"
#include "elements/menukeyselect.h"
#include "controller.h"

RedefineKeysMenu::RedefineKeysMenu(Controller *home, Controller *away, SDL_Surface *screen, std::string name) :
							Menu(screen, name),
							m_home(home),
							m_away(away)
{
	addItem(new MenuLabel(this, _("Player 1:"), "player1label"));
	addItem(m_keyHomeUp = new MenuKeySelect(this, home->keyUp(), _("Up:"), "p1_up"));
	addItem(m_keyHomeDown = new MenuKeySelect(this, home->keyDown(), _("Down:"), "p1_down"));
	addItem(m_keyHomeLeft = new MenuKeySelect(this, home->keyLeft(), _("Left:"), "p1_left"));
	addItem(m_keyHomeRight = new MenuKeySelect(this, home->keyRight(), _("Right:"), "p1_right"));
	addItem(m_keyHomePass = new MenuKeySelect(this, home->keyPass(), _("Pass:"), "p1_pass"));
	addItem(m_keyHomeShoot = new MenuKeySelect(this, home->keyShoot(), _("Shoot:"), "p1_shoot"));
	addItem(new MenuLabel(this, _("Player 2:"), "player2label"));
	addItem(m_keyAwayUp = new MenuKeySelect(this, away->keyUp(), _("Up:"), "p2_up"));
	addItem(m_keyAwayDown = new MenuKeySelect(this, away->keyDown(), _("Down:"), "p2_down"));
	addItem(m_keyAwayLeft = new MenuKeySelect(this, away->keyLeft(), _("Left:"), "p2_left"));
	addItem(m_keyAwayRight = new MenuKeySelect(this, away->keyRight(), _("Right:"), "p2_right"));
	addItem(m_keyAwayPass = new MenuKeySelect(this, away->keyPass(), _("Pass:"), "p2_pass"));
	addItem(m_keyAwayShoot = new MenuKeySelect(this, away->keyShoot(), _("Shoot:"), "p2_shoot"));
	addItem(new MenuButton(this, _("Exit"), "exit"));
}

RedefineKeysMenu::~RedefineKeysMenu()
{
	m_home->setKeyUp(m_keyHomeUp->currentKey());
	m_home->setKeyDown(m_keyHomeDown->currentKey());
	m_home->setKeyLeft(m_keyHomeLeft->currentKey());
	m_home->setKeyRight(m_keyHomeRight->currentKey());
	m_home->setKeyPass(m_keyHomePass->currentKey());
	m_home->setKeyShoot(m_keyHomeShoot->currentKey());
	m_away->setKeyUp(m_keyAwayUp->currentKey());
	m_away->setKeyDown(m_keyAwayDown->currentKey());
	m_away->setKeyLeft(m_keyAwayLeft->currentKey());
	m_away->setKeyRight(m_keyAwayRight->currentKey());
	m_away->setKeyPass(m_keyAwayPass->currentKey());
	m_away->setKeyShoot(m_keyAwayShoot->currentKey());
}
