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

#include "pitch.h"

#include <iostream>

#include "resources/surfacemanager.h"
#include "tilemap.h"
#include "logger/logger.h"

double Pitch::gravity=-0.06;

Pitch::Pitch(Graphics *renderer)
{
	m_bound.x = 200;
	m_bound.w = 2160;
	m_bound.y = 200;
	m_bound.h = 3696;

	m_friction = 0.975;

	m_goalHeight = 100;
	m_goalBoundLeft = 1260-246;
	m_goalBoundRight = 1260+246;
	m_barDist = 25;
	m_areaLeft = 756;
	m_areaRight = 1806;
	m_topAreaBottom = 700;
	m_bottomAreaTop = 3396;

	m_sixYardHeight = 150;
	m_sixYardWidth = 600;

	m_penaltySpotTop = 560;
	m_penaltySpotBottom = 3536;

	m_circleRadius = 300.0;

	m_renderer = renderer;

	if(m_renderer && (m_renderer->screen())) {
		m_pitchLines = new TileMap(m_renderer->screen(), "graphics/pitchlines");

		m_surface = SurfaceManager::instance()->load(m_renderer->screen()->format, "graphics/pitchtile.png", false, false);
		m_goalPostTop = SurfaceManager::instance()->load(m_renderer->screen()->format, "graphics/goalposttop.tga", true, false);
		m_goalPostBottom = SurfaceManager::instance()->load(m_renderer->screen()->format, "graphics/goalpostbottom.tga", true, false);
		m_scratch = SurfaceManager::instance()->load(m_renderer->screen()->format, "graphics/scratch.png", true, false);

		SDL_Rect sr = {-256, -150, 0, 0};
		SDL_Rect sh = {0,0,0,0};
		SDL_Rect so = {0,0,0,0};

		m_goalPostSpriteTop = new SpriteObject(m_goalPostTop, 0, 0,
							Point3D(centerX(), topBound()),
							sr, sh, so);

		sr.y = -110;
		m_goalPostSpriteBottom = new SpriteObject(m_goalPostBottom, 0, 0,
							Point3D(centerX(), bottomBound()),
							sr, sh, so);

		m_renderer->addSprite(m_goalPostSpriteTop);
		m_renderer->addSprite(m_goalPostSpriteBottom);

		m_width = 2560;
		m_height = 4096;
	} else {
		ERROR("Error - screen not defined at Pitch creation, cannot initialise pitch");
	}

/*	m_numScratches = 1000;
	m_scratchList = new int[m_numScratches * 2];
	for(int count=0; count<m_numScratches*2; count++) {
		m_scratchList[count] = -32;
	}
	m_curScratch = 0;*/
}

Pitch::~Pitch()
{
	SurfaceManager::instance()->release(m_surface);

	m_renderer->removeSprite(m_goalPostSpriteTop);
	m_renderer->removeSprite(m_goalPostSpriteBottom);

	delete m_goalPostSpriteTop;
	delete m_goalPostSpriteBottom;

	SurfaceManager::instance()->release(m_goalPostTop);
	SurfaceManager::instance()->release(m_goalPostBottom);
	SurfaceManager::instance()->release(m_scratch);
}

bool Pitch::inBounds(int x, int y)
{
	if(x < m_bound.x) return false;
	if(x > m_bound.x + m_bound.w) return false;
	if(y < m_bound.y) return false;
	if(y > m_bound.y + m_bound.h) return false;
	return true;
}

int Pitch::width()
{
	return m_width;
}

int Pitch::height()
{
	return m_height;
}

void Pitch::draw(int left, int top)
{
	SDL_Rect r;
	SDL_Rect s;

	if(m_surface==0) return;
	if(m_renderer==0) return;
	if(m_renderer->screen()==0) return;

	//
	// Draw the pitch tile across the screen.
	//

	int x=(left/m_surface->w)*m_surface->w - left;

	while(x< m_renderer->screen()->w) {
		int y= (top / m_surface->h) * m_surface->h - top;

		while(y < m_renderer->screen()->w) {
			r.x = 0;
			r.y = 0;
			r.w = m_surface->w;
			r.h = m_surface->h;
			s.x = x;
			s.y = y;

			if(SDL_BlitSurface(m_surface, &r, m_renderer->screen(), &s) < 0) {
				ERROR("could not pitch tile : " << SDL_GetError());
			}

			y+= m_surface->h;
		}

		x += m_surface->w;
	}

	m_pitchLines->draw(left, top);

/*	for(int count=0; count<m_numScratches; count++) {
		s.x = m_scratchList[count*2] - left;
		s.y = m_scratchList[count*2 + 1] - top;
		s.w = r.w = m_scratch->w;
		s.h = r.h = m_scratch->h;
		r.x = 0;
		r.y = 0;
		if(SDL_BlitSurface(m_scratch, &r, m_renderer->screen(), &s) < 0) {
			ERROR("could not draw pitch scratch : " << SDL_GetError());
		}
	}*/
}

Point3D Pitch::centerSpot()
{
	return Point3D(m_width/2, m_height/2);
}

Point3D Pitch::cornerSpot(bool left, bool top)
{
	return Point3D(left ? m_bound.x : m_bound.x + m_bound.w,
		       top ? m_bound.y : m_bound.y + m_bound.h);
}

Point3D Pitch::penaltySpot(bool top)
{
	return Point3D(centerX(), top ? m_penaltySpotTop : m_penaltySpotBottom);
}

double Pitch::leftBound()
{
	return m_bound.x;
}

double Pitch::rightBound()
{
	return m_bound.x + m_bound.w;
}

double Pitch::topBound()
{
	return m_bound.y;
}

double Pitch::bottomBound()
{
	return m_bound.y + m_bound.h;
}

double Pitch::centerX()
{
	return m_bound.x + (m_bound.w/2);
}

double Pitch::centerY()
{
	return m_bound.y + (m_bound.h/2);
}

double Pitch::circleRadius()
{
	return m_circleRadius;
}

bool Pitch::insideCircle(const Point3D &point)
{
	if(   (centerSpot() - point).length() < circleRadius()*circleRadius()) return true;
	return false;
}

double Pitch::friction()
{
	return m_friction;
}

double Pitch::closestPointLineSegments(Segment sega, Segment segb)
{
	Point3D u = sega.vector();
	Point3D v = segb.vector();
	Point3D w = sega.start() - segb.start();
	double a = u.dotProduct(u);        // always >= 0
	double b = u.dotProduct(v);
	double c = v.dotProduct(v);        // always >= 0
	double d = u.dotProduct(w);
	double e = v.dotProduct(w);
	double D = a*c - b*b;       // always >= 0
	double sc, sN, sD = D;
	double tc, tN, tD = D;

	if (D < 0.00000001) {         // the lines are almost parallel
		sN = 0.0;
		tN = e;
		tD = c;
	} else {
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0) {
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if(tN < 0) {
		tN = 0.0;

		if(-d < 0) sN = 0.0;
		else if (-d > a) sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	} else if (tN > tD) {
		tN = tD;
		if ((-d + b) < 0) sN = 0;
		else if ((-d + b) > a) sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}

	sc = sN / sD;
	tc = tN / tD;

	Point3D dP = w + (u*sc) - (v*tc);

	m_lastIntersection = sega.start() + (u*sc);

	return dP.length();
}

Point3D Pitch::goalPost(bool left, bool top)
{
	return Point3D( left ? m_goalBoundLeft : m_goalBoundRight,
			top ? topBound() : bottomBound() );
}

double Pitch::goalHeight()
{
	return m_goalHeight;
}

/** returns true if the line given, once projected onto the ground, crosses the sideline. Otherwise
 * returns false. the intersection Point will be set to the correct point of the line if true*/
bool Pitch::crossesSideLine(Segment move)
{
	Point3D start = move.start();
	Point3D end = move.end();
	start.setZ(0);
	end.setZ(0);

	Segment flat(start, end);

	Segment sideline(	Point3D(m_bound.x, m_bound.y),
				Point3D(m_bound.x, m_bound.y + m_bound.h));

	if(closestPointLineSegments(flat, sideline) < 0.1) {
		if(move.end().x() < m_bound.x) {
			return true;
		}
	}

	sideline = Segment(	Point3D(m_bound.x + m_bound.w, m_bound.y),
				Point3D(m_bound.x + m_bound.w, m_bound.y + m_bound.h));

	if(closestPointLineSegments(flat, sideline) < 0.1) {
		if(move.end().x() > m_bound.x + m_bound.w) {
			return true;
		}
	}


	return false;
}

/** returns true if the line given, once projected onto the ground, crosses one of the goal lines.
 * Otherwise returns false. the intersection Point will be set to the correct point of the line if
 * true */
bool Pitch::crossesGoalLine(Segment move)
{
	Point3D start = move.start();
	Point3D end = move.end();
	start.setZ(0);
	end.setZ(0);

	Segment flat(start, end);

	Segment goalline(	Point3D(m_bound.x, m_bound.y),
				Point3D(m_bound.x + m_bound.w, m_bound.y));

	if(closestPointLineSegments(flat, goalline) < 0.1) {
		if(move.end().y() < m_bound.y) {
			return true;
		}
	}

	goalline = Segment(     Point3D(m_bound.x, m_bound.y + m_bound.h),
				Point3D(m_bound.x + m_bound.w, m_bound.y + m_bound.h));

	if(closestPointLineSegments(flat, goalline) < 0.1) {
		if(move.end().y() > m_bound.y + m_bound.h) {
			return true;
		}
	}

	return false;
}

/** returns true if the line given has entered the goal mouth.
 * Otherwise returns false. the intersection Point will be set to the correct point of the line if
 * true */
bool Pitch::entersGoal(Segment move)
{
	WARN("FIXME : Ball going over the goal will stil score a goal at present");

	if(crossesGoalLine(move)) {
		if ( 	(m_lastIntersection.x() > m_goalBoundLeft) &&
			(m_lastIntersection.x() < m_goalBoundRight) &&
			(m_lastIntersection.z() < m_goalHeight)) {
			return true;
		}
	}

	return false;
}

Point3D Pitch::lastIntersection()
{
	return m_lastIntersection;
}

bool Pitch::inTopHalf(const Point3D &pos)
{
	return pos.y() < centerY();
}

bool Pitch::inLeftHalf(const Point3D &pos)
{
	return pos.x() < centerX();
}

Point3D Pitch::closestCorner(const Point3D &pos)
{
	return cornerSpot( (pos.x() < centerX()), inTopHalf(pos));
}

Point3D Pitch::closestGoalKick(const Point3D &pos)
{
	return goalKickSpot(inLeftHalf(pos), inTopHalf(pos));
}

Point3D Pitch::goalKickSpot(bool left, bool top)
{
	return Point3D( centerX() + (left ? (-m_sixYardWidth/2): (m_sixYardWidth/2)),
			top ? topBound() + m_sixYardHeight : bottomBound() - m_sixYardHeight);
}

bool Pitch::insideTheArea(const Point3D &pos, bool top)
{
	if(pos.x() < m_areaLeft) return false;
	if(pos.x() > m_areaRight) return false;

	if(top) {
		if(pos.y() < topBound()) return false;
		if(pos.y() > m_topAreaBottom) return false;
	} else {
		if(pos.y() < m_bottomAreaTop) return false;
		if(pos.y() > bottomBound()) return false;
	}
	return true;
}

void Pitch::scratchSurface(const Point3D &pos)
{
/*	m_scratchList[m_curScratch*2] = (int)pos.x();
	m_scratchList[m_curScratch*2+1] = (int)pos.y();

	m_curScratch = (m_curScratch + 1) % m_numScratches;*/
}
