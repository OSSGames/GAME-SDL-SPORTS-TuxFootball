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

#ifndef TUXFOOTBALL_MATCHSTARTSTATE
#define TUXFOOTBALL_MATCHSTARTSTATE

#include "statebase.h"

class GameEngine;

class MatchStartState : public StateBase {
public:
	MatchStartState(GameEngine &engine);
	virtual ~MatchStartState();

	/** Called when the state is entered. Occurs before the update loop method is called. */
	virtual void enterState();

	/** Called when leaving the state, after the last updateLoop has finished. */
	virtual void leaveState();

	/** The main logic for the state should be in updateLoop. */
	virtual void updateLoop();

	/** Returns true, if the gamestate is a 'in game' gamestate */
	virtual bool isGameInProgress() const;

	/** Perform rendering of a single frame for the game state */
	void renderFrame();

private:
	GameEngine &m_engine;
};
#endif /* TUXFOOTBALL_GAMEENGINE */
