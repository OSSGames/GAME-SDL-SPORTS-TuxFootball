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

#include "extratimesecondhalfstate.h"

#include <iostream>

#include "gameengine.h"

ExtraTimeSecondHalfState::ExtraTimeSecondHalfState(GameEngine &engine) :
				IngameStateBase(engine)
{
}

ExtraTimeSecondHalfState::~ExtraTimeSecondHalfState()
{
}

void ExtraTimeSecondHalfState::enterState()
{
	m_engine.team(GameEngine::HomeTeam)->changeSides();
	m_engine.team(GameEngine::AwayTeam)->changeSides();
	m_engine.setSubMode(GameEngine::KickOff, false);
	m_engine.setTimer(GameEngine::Restart);
	m_engine.setTimer(GameEngine::Stop);
}

void ExtraTimeSecondHalfState::leaveState()
{
}

void ExtraTimeSecondHalfState::updateLoop()
{
	if(m_engine.timer() > m_engine.halfLength()/3) {
		if(m_engine.score(GameEngine::HomeTeam) == m_engine.score(GameEngine::AwayTeam)) {
			m_engine.setState(GameEngine::PenaltyShootout);
		} else {
			m_engine.setState(GameEngine::Finished);
		}
		m_engine.setTimer(GameEngine::Stop);
		m_engine.playSound(GameEngine::RefWhistleEnd);
	}
}
