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

#include "fulltimestate.h"

#include <iostream>

#include "gameengine.h"
#include "resources/surfacemanager.h"
#include "logger/logger.h"

FullTimeState::FullTimeState(GameEngine &engine) :
			m_engine(engine), m_logo(0)
{
}

FullTimeState::~FullTimeState()
{
	clearLogo();
}
	
void FullTimeState::enterState()
{
	if(m_engine.score(GameEngine::HomeTeam) == m_engine.score(GameEngine::AwayTeam)) {
		m_engine.setState(GameEngine::ExtraTimeStart);
		m_engine.setTimer(GameEngine::Stop);
	} else {
		m_engine.team(GameEngine::HomeTeam)->setupHalfTime();
		m_engine.team(GameEngine::AwayTeam)->setupHalfTime();
		m_logo = SurfaceManager::instance()->load(m_engine.screen()->format, "graphics/fulltime.png", false, true);
	}
}

void FullTimeState::leaveState()
{
	clearLogo();
}

void FullTimeState::clearLogo()
{
	if(m_logo) {
		SurfaceManager::instance()->release(m_logo);
		m_logo = 0;
	}
}

void FullTimeState::updateLoop()
{
	if(m_engine.timerState() == GameEngine::Stop) {
		if(m_engine.team(GameEngine::HomeTeam)->ready() && m_engine.team(GameEngine::AwayTeam)->ready()) {
			m_engine.setTimer(GameEngine::Restart);
		}
	} else if(m_engine.timer() > 300) {
		clearLogo();
		m_engine.setState(GameEngine::Finished);
		m_engine.setTimer(GameEngine::Stop);
	}
}

bool FullTimeState::isGameInProgress() const
{
	return false;
}

void FullTimeState::renderFrame()
{
	SDL_Rect r, s;

	if(m_logo) {
		s.x = (m_engine.screen()->w - m_logo->w) / 2;
		s.y = 50;
		r.x = 0;
		r.y = 0;
		s.w = r.w = m_logo->w;
		s.h = r.h = m_logo->h;


		if(SDL_BlitSurface(m_logo, &r, m_engine.screen(), &s) < 0) {
			ERROR("could not pitch tile : " << SDL_GetError());
		}
	}
}
