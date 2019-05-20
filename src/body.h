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

#ifndef TUXFOOTBALL_BODY
#define TUXFOOTBALL_BODY

#include <SDL.h>

#include "point3d.h"

/* Represents a physical body in the game world (e.g. a player or the ball) */
class Body {
public:
	Body();
	Body(const Point3D position, const Point3D velocity, const SDL_Rect boundingBox);
	virtual ~Body();
	virtual Point3D position();
	virtual Point3D velocity();
	virtual void setPosition(const Point3D &pos);
	virtual void setVelocity(const Point3D &vel);
	virtual void accelerate(const Point3D &acc);
	virtual void applyFriction(double friction);
	virtual void move();
private:
	Point3D m_position;
	Point3D m_velocity;
	SDL_Rect m_boundingBox;
};

#endif /* TUXFOOTBALL_BODY */
