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
	
#include "nationalanthemstate.h"

#include "gameengine.h"

NationalAnthemState::NationalAnthemState(GameEngine &engine) :
			m_engine(engine)
{
}

NationalAnthemState::~NationalAnthemState()
{
}
	
void NationalAnthemState::enterState()
{
	m_engine.team(GameEngine::HomeTeam)->setupNationalAnthem();
	m_engine.team(GameEngine::AwayTeam)->setupNationalAnthem();
}

void NationalAnthemState::leaveState()
{
}

void NationalAnthemState::updateLoop()
{
	if(m_engine.timerState() == GameEngine::Stop) {
		if(m_engine.team(GameEngine::HomeTeam)->ready() && 
		   m_engine.team(GameEngine::AwayTeam)->ready()) {
			m_engine.setTimer(GameEngine::Restart);
		}
	} else if(m_engine.timer() > 100) {
		m_engine.setState(GameEngine::FirstHalf);
		m_engine.setTimer(GameEngine::Stop);
	}
}

bool NationalAnthemState::isGameInProgress() const
{
	return false;
}

void NationalAnthemState::renderFrame()
{
}
