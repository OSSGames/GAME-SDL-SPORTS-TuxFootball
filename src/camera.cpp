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

#include "camera.h"

#include <math.h>
#include <iostream>

#include "rect.h"

Camera::Camera() :
	m_accel(0.25),
	m_body(Point3D(0, 0, 0), Point3D(0, 0, 0), Rect()),
	m_dest(Point3D(0, 0, 0)),
	m_target(0)
{
}

Camera::~Camera()
{
}

void Camera::setPosition(const Point3D &pos)
{
	m_body.setPosition(pos);
}

void Camera::setTarget(Body *target)
{
	m_target = target;
}

void Camera::setDestination(const Point3D &pos)
{
	m_dest = pos;
}

void Camera::update()
{
	updateDestination();
	updatePosition();
}

void Camera::updateDestination()
{
	static double angx=0.0;
	static double angy=0.0;
	static double incx=0.001;
	static double incy=0.0013;

	if(m_target != 0) {
		setDestination(m_target->position());
	} else {
		double cx = m_boundRect.x + (m_boundRect.w/2);
		double cy = m_boundRect.y + (m_boundRect.h/2);
		setDestination(Point3D( cx + ((m_boundRect.w)/2)*sin(angx),
					cy + ((m_boundRect.h)/2)*cos(angy)));

		angx += incx;
		angy += incy;
	}
}

void Camera::updatePosition()
{
	Point3D desVel(0,0,0);
	double x, y, z;

	desVel = m_dest - m_body.position();

	x = desVel.x();
	y = desVel.y();
	z = desVel.z();

	x = (x > 0) ? sqrt(x * m_accel) : -sqrt(-x * m_accel);
	y = (y > 0) ? sqrt(y * m_accel) : -sqrt(-y * m_accel);
	z = (z > 0) ? sqrt(z * m_accel) : -sqrt(-z * m_accel);

	if((x > -m_accel) && (x < m_accel)) x=0.0;
	if((y > -m_accel) && (y < m_accel)) y=0.0;
	if((z > -m_accel) && (z < m_accel)) z=0.0;

	desVel = Point3D(x, y, z) - m_body.velocity();

	desVel = Point3D( keepInRange(desVel.x(), m_accel),
			  keepInRange(desVel.y(), m_accel),
			  keepInRange(desVel.z(), m_accel));

	m_body.accelerate(desVel);

	m_body.move();

	// check bounds

	x = m_body.position().x();
	y = m_body.position().y();

	if(x < m_boundRect.x) x = m_boundRect.x;
	if(x > m_boundRect.w + m_boundRect.x) x = m_boundRect.w + m_boundRect.x;
	if(y < m_boundRect.y) y = m_boundRect.y;
	if(y > m_boundRect.h + m_boundRect.y) y = m_boundRect.h + m_boundRect.y;
	m_body.setPosition(Point3D(x, y));
}

Point3D Camera::position()
{
	return m_body.position();
}

double Camera::keepInRange(double val, double range)
{
	// well, i felt like being clever.
	return (val > 0) ? ((val>range) ? range : val) : ((val<-range) ? -range : val);
}

void Camera::setBoundingRect(const SDL_Rect &rect)
{
	m_boundRect = rect;
}

void Camera::setScreenRect(const SDL_Rect &rect)
{
	// returns the size and position of the onscreen rectangle that the camera draws to.

	m_screenRect = rect;
}
