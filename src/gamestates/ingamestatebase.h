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

#ifndef TUXFOOTBALL_INGAMESTATEBASE
#define TUXFOOTBALL_INGAMESTATEBASE

#include "statebase.h"

class GameEngine;
class SFont;
struct SDL_Surface;

/** \brief This gamestate is the base for all game states used when the player
 * is "in game".
 *
 * Basically this means that
 * - the score is drawn
 * - the game time is drawn
 */
class IngameStateBase : public StateBase {
public:
	IngameStateBase(GameEngine &engine);
	virtual ~IngameStateBase();

	/** Returns true, because the gamestate is a 'in game' gamestate */
	virtual bool isGameInProgress() const;

	/** Perform rendering of a single frame for the game state */
	void renderFrame();

protected:
	GameEngine &m_engine;
	SDL_Surface *m_score;
	SFont* m_nameFont;
	SFont* m_scoreFont;
};

#endif /* TUXFOOTBALL_INGAMESTATEBASE */
