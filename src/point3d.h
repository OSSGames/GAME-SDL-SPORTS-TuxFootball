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

#ifndef TUXFOOTBALL_VECTOR
#define TUXFOOTBALL_VECTOR

/** A 3d point. */
class Point3D {
public:
	Point3D();
	Point3D(double x, double y, double z=0.0);
	Point3D(const Point3D &copy);
	Point3D &operator=(const Point3D &rhs);
	~Point3D();
	Point3D operator*(double k) const;
	Point3D &operator*=(double k);
	Point3D operator+(Point3D vec) const;
	Point3D &operator+=(Point3D vec);	
	Point3D operator-(Point3D vec) const;
	Point3D &operator-=(Point3D vec);	
	Point3D &setZ(double z);
	void applyArbitraryRotation(Point3D &axis, double theta);
	double x() const;
	double y() const;
	double z() const;
	double length() const;
	double dotProduct(Point3D point) const;
	Point3D normalise() const;
	bool liesWithinArc(Point3D left, Point3D right);
private:
	double m_x, m_y, m_z;
};

#endif /* TUXFOOTBALL_VECTOR */
