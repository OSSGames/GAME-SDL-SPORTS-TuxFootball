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

#ifndef TUXFOOTBALL_BALL
#define TUXFOOTBALL_BALL

#include "body.h"

class Graphics;
class Pitch;
class Player;
class SpriteObject;
struct Mix_Chunk;

/** Contains all functioanlity related to the football. */
class Ball : public Body {
public:
	Ball(Graphics *renderer, Pitch *pitch);
	virtual ~Ball();
	Point3D lastPosition();
	void move();
	void kickBall(const Point3D &vel, int priority, Player *kickBy);
	void applyKick();

	/**Calculate the velocity required for the ball to reach and stop at end */
	Point3D calculateReqVelocity(double z, Point3D end);
private:
	Pitch *m_pitch;
	Graphics *m_renderer;
	Point3D m_lastPosition;
	SDL_Surface *m_football;
	SDL_Surface *m_shadow;
	SpriteObject *m_object;
	static double bounceFactor;
	
	Point3D m_kickVel;
	Player *m_kickBy;
	int m_kickPriority;

	// sound fx
	Mix_Chunk *m_bounce;
	Mix_Chunk *m_kick;
	Mix_Chunk *m_rebound;
};

#endif  /* TUXFOOTBALL_BALL */
