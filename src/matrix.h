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

#ifndef TUXFOOTBALL_MATRIX
#define TUXFOOTBALL_MATRIX

#include "point3d.h"

/** Matrix math operations. */
class Matrix {
public:
	Matrix();
	~Matrix();

	Matrix operator*(const Matrix &mat);
	Matrix& operator*=(const Matrix &mat);
	Point3D operator*(const Point3D &vec);

	static Matrix identity();
	static Matrix arbitraryTransform(const Point3D &dir, double angle);
	static Matrix pitchTransform(double pitch);
	static Matrix rollTransform(double roll);
	static Matrix yawTransform(double yaw);

	Matrix inverse();

	double val(int x, int y) const;
	void setVal(int x, int y, double val);
#if 0
private:
	void correct();
#endif
private:
	double m_array[16];
};

#endif // TUXFOOTBALL_MATRIX
