/***************************************************************************
 *   Copyright (C) 2003-2011 by Tux Football development team              *
 *   Authors: Christoph Brill <egore911@egore911.de>                       *
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

#ifndef TUXFOOTBALL_MENUSTATEBASE
#define TUXFOOTBALL_MENUSTATEBASE

#include "statebase.h"

#include <SDL.h>

class GameEngine;
class SFont;
class Menu;

/** \brief This gamestate is the base for all game states used when the player
 * is "in game".
 *
 * Basically this means that
 * - the score is drawn
 * - the game time is drawn
 */
class MenuStateBase : public StateBase {
public:
	MenuStateBase(GameEngine &engine);
	virtual ~MenuStateBase();

	/** Called when the state is entered. Occurs before the update loop method is called. */
	virtual void enterState();

	/** Returns false, because the gamestate is a 'menu' gamestate */
	bool isGameInProgress() const;

	/** Perform rendering of a single frame for the game state */
	virtual void renderFrame();

	void update(Uint8* keys);

protected:
	GameEngine &m_engine;
	SDL_Surface *m_logo;

	virtual void initialiseMenu() = 0;
	/** clears the logo, if one exists. It is safe to call this when no logo exists. */
	void clearLogo();
	void setMenu(Menu *menu);
	Menu* menu();
private:
	Menu *m_menu;
};

#endif /* TUXFOOTBALL_MENUSTATEBASE */
