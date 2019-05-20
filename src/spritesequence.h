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

#ifndef TUXFOOTBALL_SPRITESEQUENCE
#define TUXFOOTBALL_SPRITESEQUENCE

#include <list>
#include <string>

#include "SDL.h"

struct Sprite {
	SDL_Surface *surface[8];
	int duration;
};

/** A sequence of sprite objects which can be played one after another to form
 * an animation. */
class SpriteSequence {
public:
	SpriteSequence(SDL_PixelFormat *format, std::string dirname, std::string animname, int animationFrames, bool repeats);
	~SpriteSequence();
	SDL_Surface *surface(int direction);
	void updateSequence();
	void restartSequence();
private:
	SDL_PixelFormat *m_format;
	std::list<Sprite> m_seq;
	std::list<Sprite>::iterator m_current;
	int m_spriteTime;
	bool m_repeats;
};

#endif /* TUXFOOTBALL_SPRITESEQUENECE */
