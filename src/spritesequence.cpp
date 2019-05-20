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

#include "spritesequence.h"

#include <iostream>
#include <sstream>
#include <stdio.h>

#include "resources/surfacemanager.h"

SpriteSequence::SpriteSequence(SDL_PixelFormat *format, std::string dirname, std::string animname, int animationFrames, bool repeats)
{
	m_format = format;
	m_repeats = repeats;

	std::list<std::string> subdirs;
	subdirs.push_back("s");
	subdirs.push_back("se");
	subdirs.push_back("e");
	subdirs.push_back("ne");
	subdirs.push_back("n");
	subdirs.push_back("nw");
	subdirs.push_back("w");
	subdirs.push_back("sw");
	for(int count=0; count<animationFrames; count++) {
		Sprite sprite;
		int direction = 0;
		std::list<std::string>::const_iterator ci = subdirs.begin();
		for (; ci != subdirs.end(); ++ci) {
			std::ostringstream ost;
			ost << (count + 1);

			std::string filename = ost.str();

			while(filename.length() < 4) {
				filename = "0" + filename;
			}
			filename = dirname + "/" + *ci + "/" + animname + filename + ".png";

			sprite.surface[direction] = SurfaceManager::instance()->load(m_format, filename, true, false);
			direction++;
		}
		sprite.duration = 1;
		m_seq.push_back(sprite);
		direction++;
	}

	m_current = m_seq.begin();
	m_spriteTime = (*m_current).duration;
}

SpriteSequence::~SpriteSequence()
{
	std::list<Sprite>::iterator itt;

	for(itt = m_seq.begin(); itt!=m_seq.end(); ++itt) {
		for(int count = 0; count<8; count++) {
			SurfaceManager::instance()->release((*itt).surface[count]);
		}
	}
}


SDL_Surface *SpriteSequence::surface(int currentDirection)
{
	return (*m_current).surface[currentDirection];
}

void SpriteSequence::updateSequence()
{
	m_spriteTime --;

	if(!m_spriteTime) {
		++m_current;
		if(m_current==m_seq.end()) {
			if(m_repeats) {
				m_current=m_seq.begin();
			} else {
				--m_current;
			}
		}
		m_spriteTime = (*m_current).duration;
	}
}

void SpriteSequence::restartSequence()
{
	m_current=m_seq.begin();
	m_spriteTime = (*m_current).duration;	
}
