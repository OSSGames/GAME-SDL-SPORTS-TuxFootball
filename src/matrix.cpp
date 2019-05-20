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

#include "matrix.h"

#include <cmath>

Matrix::Matrix()
{
  m_array[0]=1.0;
  m_array[1]=0.0;
  m_array[2]=0.0;
  m_array[3]=0.0;
  m_array[4]=0.0;
  m_array[5]=1.0;
  m_array[6]=0.0;
  m_array[7]=0.0;
  m_array[8]=0.0;
  m_array[9]=0.0;
  m_array[10]=1.0;
  m_array[11]=0.0;
  m_array[12]=0.0;
  m_array[13]=0.0;
  m_array[14]=0.0;
  m_array[15]=1.0;
}

Matrix::~Matrix()
{
}

Matrix Matrix::operator*(const Matrix &mat)
{
	Matrix ret;

	for(int x=0; x<4; x++) {
		for(int y=0; y<4; y++) {
			ret.setVal(x, y,	mat.val(0, y)*val(x, 0) +
						mat.val(1, y)*val(x, 1) +
						mat.val(2, y)*val(x, 2) +
						mat.val(3, y)*val(x, 3));
		}
	}

	return ret;
}

Matrix& Matrix::operator*=(const Matrix &mat)
{
	*this = *this * mat;
	return *this;
}

Point3D Matrix::operator*(const Point3D &vec)
{
	return Point3D(	vec.x() * val(0,0) +
				vec.y() * val(1,0) +
				vec.z() * val(2,0),
			vec.x() * val(0, 1) +
				vec.y() * val(1, 1) +
				vec.z() * val(2, 1),
			vec.x() * val(0, 2) +
				vec.y() * val(1, 2) +
				vec.z() * val(2, 2));
}

Matrix Matrix::identity()
{
	Matrix mat;

	return mat;
}

Matrix Matrix::arbitraryTransform(const Point3D &dir, double angle)
{
	Matrix m;

	double cosine = cos(angle);
	double sine = sin(angle);
	double ocos = 1 - cosine;
	double x = dir.x();
	double y = dir.y();
	double z = dir.z();

	m.setVal(0, 0, x*x*ocos + cosine);
	m.setVal(1, 0, x*y*ocos + z*sine);
	m.setVal(2, 0, x*z*ocos + y*sine);
	m.setVal(0, 1, y*x*ocos + z*sine);
	m.setVal(1, 1, y*y*ocos + cosine);
	m.setVal(2, 1, y*z*ocos - x*sine);
	m.setVal(0, 2, z*x*ocos - y*sine);
	m.setVal(1, 2, z*y*ocos + x*sine);
	m.setVal(2, 2, z*z*ocos + cosine);

	return m;
}

Matrix Matrix::pitchTransform(double pitch)
{
	Matrix ret;

	ret.setVal(1,1, cos(pitch));
	ret.setVal(2,1, -sin(pitch));
	ret.setVal(1,2, sin(pitch));
	ret.setVal(2,2, cos(pitch));

	return ret;
}

Matrix Matrix::rollTransform(double roll)
{
	Matrix ret;

	ret.setVal(0,0, cos(roll));
	ret.setVal(1,0, -sin(roll));
	ret.setVal(0,1, sin(roll));
	ret.setVal(1,1, cos(roll));

	return ret;
}

Matrix Matrix::yawTransform(double yaw)
{
	Matrix ret;

	ret.setVal(0,0, cos(yaw));
	ret.setVal(2,0, sin(yaw));
	ret.setVal(0,2, -sin(yaw));
	ret.setVal(2,2, cos(yaw));

	return ret;
}

Matrix Matrix::inverse()
{
  Matrix inv;

  inv.setVal(0,0, m_array[0]);
  inv.setVal(1,0, m_array[4]);
  inv.setVal(2,0, m_array[8]);
  inv.setVal(0,1, m_array[1]);
  inv.setVal(1,1, m_array[5]);
  inv.setVal(2,1, m_array[9]);
  inv.setVal(0,2, m_array[2]);
  inv.setVal(1,2, m_array[6]);
  inv.setVal(2,2, m_array[10]);

  return inv;
}

#if 0
void Matrix::correct()
{
  double x, y, z, len;

  //
  // Corrects a m_arrayrix, making it orthonormal. Should only be
  // used for rotation m_arrayrices - loses translation
  // Uses Gram-Schmidt process, with simplification for third vector
  //

  len=sqrt(m_array[0]*m_array[0] + m_array[1]*m_array[1] + m_array[2]*m_array[2]);
  len=1/len;
  m_array[0]*=len;
  m_array[1]*=len;
  m_array[2]*=len;

  x=m_array[4] - (m_array[4]*m_array[0] + m_array[5]*m_array[1] + m_array[6]*m_array[2]) * m_array[0];
  y=m_array[5] - (m_array[4]*m_array[0] + m_array[5]*m_array[1] + m_array[6]*m_array[2]) * m_array[1];
  z=m_array[6] - (m_array[4]*m_array[0] + m_array[5]*m_array[1] + m_array[6]*m_array[2]) * m_array[2];
  len=sqrt(x*x + y*y + z*z);
  len=1/len;
  m_array[4]=x*len;
  m_array[5]=y*len;
  m_array[6]=z*len;

  m_array[8]=m_array[1]*m_array[6]-m_array[2]*m_array[5];
  m_array[9]=m_array[2]*m_array[4]-m_array[0]*m_array[6];
  m_array[10]=m_array[0]*m_array[5]-m_array[1]*m_array[4];
  x=m_array[8];
  y=m_array[9];
  z=m_array[10];
  len=sqrt((x*x)+(y*y)+(z*z));
  len=1/len;
  x*=len;
  y*=len;
  z*=len;
  m_array[8]=x;
  m_array[9]=y;
  m_array[10]=z;

  m_array[3]=m_array[7]=m_array[11]=m_array[12]=m_array[13]=m_array[14]=0;
  m_array[15]=1;
}
#endif

void Matrix::setVal(int x, int y, double val)
{
	m_array[x + y*4] = val;
}


double Matrix::val(int x, int y) const
{
	return m_array[x + y*4];
}

/*
inline void NegTranslateMatrix(double *m1, struct coordinate *t)
{
  double tmat[16];
   tmat[0]=1;  tmat[1]=0;  tmat[2]=0;  tmat[3]=-t->x;
   tmat[4]=0;  tmat[5]=1;  tmat[6]=0;  tmat[7]=-t->y;
   tmat[8]=0;  tmat[9]=0; tmat[10]=1; tmat[11]=-t->z;
  tmat[12]=0; tmat[13]=0; tmat[14]=0; tmat[15]=1;
  MultiplyMatrices(m1, tmat);
}

inline void TranslateMatrix(double *m1, struct coordinate *t)
{
  double tmat[16];
   tmat[0]=1;  tmat[1]=0;  tmat[2]=0;  tmat[3]=t->x;
   tmat[4]=0;  tmat[5]=1;  tmat[6]=0;  tmat[7]=t->y;
   tmat[8]=0;  tmat[9]=0; tmat[10]=1; tmat[11]=t->z;
  tmat[12]=0; tmat[13]=0; tmat[14]=0; tmat[15]=1;
  MultiplyMatrices(m1, tmat);
}
*/
