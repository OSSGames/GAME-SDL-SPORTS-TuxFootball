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

#ifndef TUXFOOTBALL_CAMERA
#define TUXFOOTBALL_CAMERA

#include "body.h"
#include "point3d.h"
#include "SDL.h"

/** Defines a camera, including routines to smoothly move it around the
 * pitch. */
class Camera {
public:
	Camera();
	~Camera();

	void setPosition(const Point3D &pos);
	void setDestination(const Point3D &pos);
	void update();
	Point3D position();
	void setBoundingRect(const SDL_Rect &rect);
	void setScreenRect(const SDL_Rect &rect);
	void setTarget(Body *target);
private: // private attributes
	double m_accel;

	Body m_body;

	Point3D m_dest;

	/** If set, the camera will follow this target. */
	Body *m_target;

	SDL_Rect m_boundRect;
	SDL_Rect m_screenRect;

private: // private methods
	void updateDestination();
	void updatePosition();
	double keepInRange(double val, double range);
};


#endif /* TUXFOOTBALL_CAMERA */
