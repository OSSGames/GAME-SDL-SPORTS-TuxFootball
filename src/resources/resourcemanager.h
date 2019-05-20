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

#ifndef TUXFOOTBALL_RESOURCEMANAGER
#define TUXFOOTBALL_RESOURCEMANAGER

#include <string>
#include <map>

/** Template class that keeps track of files, loading on demand and making
 * sure they are deleted as needed. */
template<class T, typename C> class ResourceManager {
public:
	virtual T *load(std::string filename) = 0;
	virtual void release(T *file) = 0;
	static C* instance () {
		if (!_instance)
			_instance = new C ();
		return _instance;
	}
protected:
	virtual T *add(std::string filename) = 0;

	static C *_instance;
	ResourceManager() {};
	ResourceManager(const ResourceManager&);
	virtual ~ResourceManager() {
		_instance = 0;
	}
private:
	ResourceManager& operator = (const ResourceManager&);
};

template <class T, typename C> C* ResourceManager <T,C>::_instance = 0;


#endif /* TUXFOOTBALL_RESOURCEMANAGER */
