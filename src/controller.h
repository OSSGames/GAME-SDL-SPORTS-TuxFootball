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

#ifndef TUXFOOTBALL_CONTROLLER
#define TUXFOOTBALL_CONTROLLER

#include <SDL.h>

class Ball;
class Pitch;
class Player;
class Team;

/** Player and CPU control. */
class Controller {
public:
	Controller(Team *team, Team *opp, SDLKey up, SDLKey down, SDLKey left, SDLKey right, SDLKey shoot, SDLKey pass, bool isCPU, Ball *ball, Pitch *pitch);
	~Controller();
	void updateController(Uint8 *keys);
	void updateComputer();
	SDLKey keyUp();
	SDLKey keyDown();
	SDLKey keyLeft();
	SDLKey keyRight();
	SDLKey keyPass();
	SDLKey keyShoot();
	void setKeyUp(SDLKey key);
	void setKeyDown(SDLKey key);
	void setKeyLeft(SDLKey key);
	void setKeyRight(SDLKey key);
	void setKeyPass(SDLKey key);
	void setKeyShoot(SDLKey key);
	void setCPU(bool cpu);
private:
	int m_shootCount;
	bool m_canKick;
	Player *m_lastPlayer;
	Team *m_team;
	Team *m_oppTeam;
	SDLKey m_up;
	SDLKey m_down;
	SDLKey m_left;
	SDLKey m_right;
	SDLKey m_shoot;
	SDLKey m_pass;
	bool m_isCPU;
	Ball *m_ball;
	Pitch *m_pitch;
	/** a counter saying how long ago we last passed the ball. used by the computer controller */
	int m_lastPass;

	int calculateBestPass(Player *player);
	double calculateClosestOpponent(Player *player);
};

#endif /* TUXFOOTBALL_CONTROLLER */
