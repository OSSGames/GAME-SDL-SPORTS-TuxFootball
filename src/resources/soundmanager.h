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

#ifndef TUXFOOTBALL_SOUNDMANAGER
#define TUXFOOTBALL_SOUNDMANAGER

#include "resourcemanager.h"

#include <SDL_mixer.h>
#include <string>
#include <map>

struct ManagedSound {
	Mix_Chunk *sound;
	int refCount;
};

class SoundManager : public ResourceManager<Mix_Chunk, SoundManager> {
public:
	virtual Mix_Chunk *load(std::string filename);
	virtual void release(Mix_Chunk *file);
protected:
	virtual Mix_Chunk *add(std::string filename);
private:
	static std::map<std::string, ManagedSound> m_sounds;
};

#endif /* TUXFOOTBALL_SOUNDMANAGER */
