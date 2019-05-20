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

#include "body.h"

#include <SDL.h>

#include "point3d.h"

Body::Body()
{
}

Body::Body(const Point3D position, const Point3D velocity, const SDL_Rect boundingBox) :
		m_position(position),
		m_velocity(velocity),
		m_boundingBox(boundingBox)
{
}

Body::~Body()
{
}

Point3D Body::position()
{
	return m_position;
}

Point3D Body::velocity()
{
	return m_velocity;
}

void Body::move()
{
	m_position += m_velocity;
}
void Body::setPosition(const Point3D &pos)
{
	m_position = pos;
}

void Body::setVelocity(const Point3D &vel)
{
	m_velocity = vel;
}

void Body::accelerate(const Point3D &acc)
{
	m_velocity += acc;
}

void Body::applyFriction(double friction)
{
	m_velocity = Point3D(	m_velocity.x()*friction,
				m_velocity.y()*friction,
				m_velocity.z());
}
