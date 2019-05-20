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

#ifndef TUXFOOTBALL_PITCH
#define TUXFOOTBALL_PITCH

#include "SDL.h"

#include "segment.h"
#include "body.h"
#include "graphics.h"
#include "spriteobject.h"
#include "point3d.h"

class TileMap;

/** Encapsulates everything to do with the pitch. */
class Pitch {
public:
	Pitch(Graphics *renderer);
	~Pitch();
	/** Returns true if the point given is on the pitch, false otherwise */
	bool inBounds(int x, int y);
	/** Returns the width of the pitch, including the area out of bounds */
	int width();
	/** Returns the height of the pitch, including the area out of bounds */
	int height();
	/** Draws an area of the pitch, starting at left, top onto the screen rectangle defined by screen,
	 * the screen surface is passed in surface. */
	void draw(int left, int top);

	Point3D centerSpot();
	Point3D cornerSpot(bool left, bool top);
	Point3D penaltySpot(bool top);
	Point3D goalPost(bool left, bool top);
	double leftBound();
	double rightBound();
	double topBound();
	double bottomBound();
	double centerX();
	double centerY();
	double circleRadius();
	bool insideCircle(const Point3D &point);
	double friction();
	double closestPointLineSegments(Segment sega, Segment segb);
	double goalHeight();
	/** returns true if the line given, once projected onto the ground, crosses the sideline. Otherwise
	 * returns false. the intersection Point will be set to the correct point of the line if true*/
	bool crossesSideLine(Segment move);
	/** returns true if the line given, once projected onto the ground, crosses one of the goal lines.
	 * Otherwise returns false. the intersection Point will be set to the correct point of the line if
	 * true */
	bool crossesGoalLine(Segment move);
	/** returns true if the line given has entered the goal mouth.
	 * Otherwise returns false. the intersection Point will be set to the correct point of the line if
	 * true */
	bool entersGoal(Segment move);
	Point3D lastIntersection();
	bool inTopHalf(const Point3D &pos);
	bool inLeftHalf(const Point3D &pos);
	Point3D closestCorner(const Point3D &pos);
	Point3D closestGoalKick(const Point3D &pos);
	Point3D goalKickSpot(bool left, bool top);
	bool insideTheArea(const Point3D &pos, bool top);
	/** "Scratches" the surface at this point. This would usually happen due to a tackle, of some similar
	 * event. The result is dependant on the pitch, but "mud splashing and leaving a furrow" can be considered
	 * a likely candidate */
	void scratchSurface(const Point3D &pos);
public:
	static double gravity;
private:
	int m_width, m_height;
	SDL_Rect m_bound;
	SDL_Surface *m_surface;
	TileMap *m_pitchLines;
	Graphics *m_renderer;

	double m_goalBoundLeft;
	double m_goalBoundRight;
	double m_goalHeight;
	double m_barDist;
	double m_friction;
	double m_areaLeft;
	double m_areaRight;
	double m_topAreaBottom;
	double m_bottomAreaTop;
	double m_sixYardHeight;
	double m_sixYardWidth;
	double m_penaltySpotTop;
	double m_penaltySpotBottom;
	double m_circleRadius;
	SDL_Surface *m_goalPostTop;
	SDL_Surface *m_goalPostBottom;
	SDL_Surface *m_scratch;

/*	int m_numScratches;
	int *m_scratchList;
	int m_curScratch;*/

	SpriteObject *m_goalPostSpriteTop;
	SpriteObject *m_goalPostSpriteBottom;

	Point3D m_lastIntersection;
};

#endif /* TUXFOOTBALL_PITCH */
