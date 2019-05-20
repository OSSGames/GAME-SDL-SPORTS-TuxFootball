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

#include "team.h"

#include <string>
#include <iostream>

#include "gameengine.h"
#include "matrix.h"

Team::Team(GameEngine *gameEngine, std::string name, std::string shortname, std::string skin, std::string playerMarker, Pitch *pitch, Ball *ball, bool topHalf)
{
	m_name = name;
	m_shortname = shortname;
	m_firstPlayer = 0;
	m_secondPlayer = 0;

	m_ball = ball;

	for(int count=0; count<11; count++) {
		Player *p = gameEngine->addPlayer(skin, playerMarker, this, (count==0));

		p->setPosition( Point3D(1024 + (count*48), 1024, 0));
		p->setDestination( Point3D(278 + (count*48), 1024, 0));

		m_players.push_back(PlayerContainer(p));
	}

	m_pitch = pitch;
	m_topHalf = topHalf;
	m_activePlayer = 0;

	setFormation(4, 3, 3);
	setupNationalAnthem();
}

Team::~Team()
{
}

void Team::update(void)
{
	list<PlayerContainer>::iterator itt, closest;

	itt = m_players.begin();
	closest = m_players.end();

	while(itt!= m_players.end()) {
		(*(*itt))->update();

		if(controlState()!=Player::None) {
			if(!(*(*itt))->committedToMove()) {

				if( (!(*(*itt))->isGoalie()) ||
				    ((*(*itt))->touchingBall(true))) {
					if(closest==m_players.end()) {
						closest = itt;
					} else if((*(*closest))->distanceFrom(m_ball->position()) > (*(*itt))->distanceFrom(m_ball->position())) {
						closest = itt;
					}
				}
			}
		}

		++itt;
	}

	switch(controlState()) {
		default:
		case Player::None :
			break;
		case Player::KickOnly :
			setActivePlayer(closest);
			break;
		case Player::Full :
			setActivePlayer(closest);
			calculateDesirablePositions();
			break;
	}
}

void Team::setFormation(int defense, int midfield, int attack)
{
	m_defenders = defense;
	m_midfielders = midfield;
	m_attackers = attack;

	while(m_defenders+m_midfielders+m_attackers > 10) {
		if(m_defenders > m_midfielders) {
			if(m_defenders > m_attackers) {
				m_defenders--;
			} else {
				m_attackers--;
			}
		} else if(m_midfielders > m_attackers) {
			m_midfielders--;
		} else {
			m_attackers--;
		}
	}
}

void Team::setupCorner(bool left, bool attack)
{
	int count;
	double spacing, y;
	Point3D playerPos, inc;

	double top = (!attack == m_topHalf) ?   m_pitch->topBound() : m_pitch->bottomBound();
        double goal = m_topHalf ? m_pitch->topBound()+32 : m_pitch->bottomBound()-32;
	Point3D corner = m_pitch->cornerSpot(left, attack ? !m_topHalf : m_topHalf);

	list<PlayerContainer>::iterator itt;
	itt = m_players.begin();

	setControlState(Player::None);

	// goalkeeper
	if(itt==m_players.end()) return;
	(*(*itt))->setDestination(Point3D( m_pitch->centerX(), goal));
	++itt;

	// defenders - inside 6 yard area.
	if(attack) {
		spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_defenders);
		y = m_pitch->centerY();

	} else {
		spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_defenders);
		y = m_pitch->centerY() + (top - m_pitch->centerY()) * 0.95;
	}

	playerPos = Point3D( m_pitch->centerX() - ((m_defenders-1)/2.0)*spacing, y);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_defenders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}

	// midfielders

	if(attack) {
		spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_midfielders);
		y = m_pitch->centerY() +  (top - m_pitch->centerY()) * 0.75;
	} else {
		spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_midfielders);
		y = m_pitch->centerY() +  (top - m_pitch->centerY()) * 0.85;
	}

	playerPos = Point3D( m_pitch->centerX() - ((m_midfielders-1)/2.0)*spacing, y);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_midfielders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}


	// attackers
	if(attack) {
		spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_attackers);
		y = m_pitch->centerY() +  (top - m_pitch->centerY()) * 0.85;
	} else {
		spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_attackers);
		y = m_pitch->centerY() +  (top - m_pitch->centerY()) * 0.7;
	}

	playerPos = Point3D( m_pitch->centerX() - ((m_attackers-1)/2.0)*spacing, y);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_attackers; count++) {
		if(itt==m_players.end()) return;

		if(m_pitch->insideCircle(playerPos)) {
			(*(*itt))->setDestination(	Point3D(playerPos.x(),
							m_topHalf ? m_pitch->centerSpot().y() - m_pitch->circleRadius() :
									m_pitch->centerSpot().y() + m_pitch->circleRadius()));
		} else {
			(*(*itt))->setDestination(playerPos);
		}

		playerPos += inc;
		++itt;
	}

	// if on the attack, move closest player to the corner spot.

	if(attack) {
		calculateClosestPlayers(m_pitch->centerSpot());
		m_firstPlayer->setDestination(corner);
	}

}

void Team::setupKickoff(bool attack)
{
	int count;
	double top = m_topHalf ? m_pitch->topBound() : m_pitch->bottomBound();
	double goal = m_topHalf ? m_pitch->topBound()+32 : m_pitch->bottomBound()-32;

	list<PlayerContainer>::iterator itt;

	itt = m_players.begin();

	setControlState(Player::None);

	// goalkeeper
	if(itt==m_players.end()) return;

	(*(*itt))->setDestination(Point3D( m_pitch->centerX(), goal));

	++itt;


	// defenders

	double spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_defenders);
	double y = m_pitch->centerY() + (top - m_pitch->centerY()) * 0.8;

	Point3D playerPos( m_pitch->centerX() - ((m_defenders-1)/2.0)*spacing, y);
	Point3D inc( spacing, 0);

	for(count = 0; count<m_defenders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}

	// midfielders
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_midfielders);

	y = m_pitch->centerY() +  (top - m_pitch->centerY()) * 0.4;

	playerPos = Point3D( m_pitch->centerX() - ((m_midfielders-1)/2.0)*spacing, y);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_midfielders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}


	// attackers
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_attackers);

	y = m_pitch->centerY() +  (top - m_pitch->centerY()) * 0.1;

	playerPos = Point3D( m_pitch->centerX() - ((m_attackers-1)/2.0)*spacing, y);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_attackers; count++) {
		if(itt==m_players.end()) return;

		if(m_pitch->insideCircle(playerPos)) {
			(*(*itt))->setDestination(	Point3D(playerPos.x(),
							m_topHalf ? m_pitch->centerSpot().y() - m_pitch->circleRadius() :
									m_pitch->centerSpot().y() + m_pitch->circleRadius()));
		} else {
			(*(*itt))->setDestination(playerPos);
		}

		playerPos += inc;
		++itt;
	}

	// if on the "attack", put two closest players near the ball.
	if(attack) {
		calculateClosestPlayers(m_pitch->centerSpot());
		m_firstPlayer->setDestination(m_pitch->centerSpot());
		m_secondPlayer->setDestination(m_pitch->centerSpot() - Point3D(100, 0,0));
	}
}

void Team::setupHalfTime()
{
	list<PlayerContainer>::iterator itt;

	Point3D playPos = m_pitch->centerSpot();

	playPos = Point3D(m_pitch->width()+32, playPos.y(), playPos.z());

	for(list<PlayerContainer>::iterator itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		(*(*itt))->setDestination(playPos);
		playPos += Point3D(32, 0);
	}

	setControlState(Player::None);
}

void Team::setupExtraTime()
{
}

void Team::setupNationalAnthem()
{
	Point3D playPos = m_pitch->centerSpot();

	playPos += Point3D(-176 , m_topHalf ? -32 : 32);

	for(list<PlayerContainer>::iterator itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		(*(*itt))->setDestination(playPos);
		playPos += Point3D(32, 0);
	}

	setControlState(Player::None);
}

void Team::changeSides()
{
	m_topHalf = !m_topHalf;
}

void Team::setControlState(Player::ControlState state)
{
	m_controlState = state;

	for(list<PlayerContainer>::iterator itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		(*(*itt))->setControlState(state);
	}

	if(state==Player::None) {
		setActivePlayer(m_players.end());
	}
}

bool Team::ready()
{
	if(controlState()!=Player::None) return true;

	for(list<PlayerContainer>::iterator itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		if( !(*(*itt))->inPosition() )	return false;
	}

	return true;
}


void Team::setPitch(Pitch *pitch)
{
	m_pitch = pitch;
}

void Team::setActivePlayer(list<PlayerContainer>::iterator player)
{
	for(list<PlayerContainer>::iterator itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		(*(*itt))->setActive(itt==player);
	}

	if( player==m_players.end()) {
		m_activePlayer = 0;
	} else {
		m_activePlayer = (*(*player));
	}
}

Player *Team::activePlayer()
{
	return m_activePlayer;
}

Player::ControlState Team::controlState()
{
	return m_controlState;
}

void Team::calculateDesirablePositions()
{
	double goal, def, mid, att;
	double spacing;
	int count;
	double ourEnd = m_topHalf ? m_pitch->topBound() : m_pitch->bottomBound();
	double theirEnd = m_topHalf ? m_pitch->bottomBound() : m_pitch->topBound();

	goal = m_topHalf ? m_pitch->topBound()+32 : m_pitch->bottomBound()-32;
	def = m_ball->position().y() + (ourEnd - m_ball->position().y()) * 0.5;
	mid = m_ball->position().y() + (ourEnd - m_ball->position().y()) * 0.2;
	att = m_ball->position().y() + (theirEnd - m_ball->position().y()) * 0.1;

	std::list<PlayerContainer>::iterator itt;

	itt = m_players.begin();

	// goalkeeper
	if(itt==m_players.end()) return;

	(*(*itt))->setDesiredPosition(Point3D(m_pitch->centerX(), goal));

	// defenders
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_defenders);
	Point3D playerPos( m_pitch->centerX() - ((m_defenders-1)/2.0)*spacing, def);
	Point3D inc( spacing, 0);

	for(count=0; count<m_defenders; count++) {
		++itt;
		if(itt==m_players.end()) return;
		(*(*itt))->setDesiredPosition(playerPos);
		playerPos+=inc;
	}

	// midfielders
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_midfielders);
	playerPos = Point3D( m_pitch->centerX() - ((m_midfielders-1)/2.0)*spacing, mid);
	inc = Point3D( spacing, 0);

	for(count=0; count<m_midfielders; count++) {
		++itt;
		if(itt==m_players.end()) return;
		(*(*itt))->setDesiredPosition(playerPos);
		playerPos+=inc;
	}

	// attackers
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_attackers);
	playerPos = Point3D( m_pitch->centerX() - ((m_attackers-1)/2.0)*spacing, att);
	inc = Point3D( spacing, 0);

	for(count=0; count<m_attackers; count++) {
		++itt;
		if(itt==m_players.end()) return;
		(*(*itt))->setDesiredPosition(playerPos);
		playerPos+=inc;
	}
}


std::string Team::name() {
	return m_name;
}

std::string Team::shortname() {
	return m_shortname;
}


bool Team::topHalf()
{
	return m_topHalf;
}

Player *Team::calculateClosestPlayers(Point3D point)
{
	double flen = 0;
	double slen = 0;

	m_firstPlayer = 0;
	m_secondPlayer = 0;

	std::list<PlayerContainer>::iterator itt;

	for(itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		double testlen = (*(*itt))->distanceFrom(point);

		if(m_firstPlayer==0) {
			m_firstPlayer = (*(*itt));
			flen = testlen;
		} else 	if(testlen < flen) {
			m_secondPlayer = m_firstPlayer;
			slen = flen;
			m_firstPlayer = (*(*itt));
			flen = testlen;
		} else if(m_secondPlayer == 0) {
			m_secondPlayer = (*(*itt));
			slen = testlen;
		} else if(testlen < slen) {
			m_secondPlayer = (*(*itt));
			slen = testlen;
		}
	}

	return m_firstPlayer;
}

void Team::setupPenalty(bool attack)
{
}

void Team::setupGoalKick(bool attack, bool left, bool onFloor)
{
	int count;
	double spacing;
	Point3D playerPos, inc;

	double ourEnd = m_topHalf ? m_pitch->topBound() : m_pitch->bottomBound();
	double theirEnd = m_topHalf ? m_pitch->bottomBound() : m_pitch->topBound();

	double attLine = ourEnd + (theirEnd - ourEnd)*0.7;
	double midLine = ourEnd + (theirEnd - ourEnd)*0.45;
	double defLine = ourEnd + (theirEnd - ourEnd)*0.2;

	setControlState(Player::None);

	std::list<PlayerContainer>::iterator itt;
	itt = m_players.begin();

	// goalkeeper
	if(itt==m_players.end()) return;
	if(onFloor) {
		(*(*itt))->setDestination(m_pitch->goalKickSpot(left, m_topHalf));
	} else {
		(*(*itt))->setDestination((*(*itt))->position());
	}
	++itt;


	// defenders
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_defenders);
	playerPos = Point3D(m_pitch->centerX() - ((m_defenders-1)/2.0)*spacing, defLine);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_defenders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}

	// midfielders
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_midfielders);
	playerPos = Point3D(m_pitch->centerX() - ((m_midfielders-1)/2.0)*spacing, midLine);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_midfielders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}

	// attackers
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_attackers);
	playerPos = Point3D(m_pitch->centerX() - ((m_attackers-1)/2.0)*spacing, attLine);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_attackers; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}
}


void Team::setupThrowIn(const Point3D &pos, bool attack)
{
	int count;
	double spacing;
	Point3D playerPos, inc;

	double ourEnd = m_topHalf ? m_pitch->topBound() : m_pitch->bottomBound();

	double mid = pos.y();
	double def = pos.y() + (ourEnd - pos.y())*0.3;

        double goal = m_topHalf ? m_pitch->topBound()+32 : m_pitch->bottomBound()-32;

	setControlState(Player::None);

	list<PlayerContainer>::iterator itt;
	itt = m_players.begin();

	// goalkeeper
	if(itt==m_players.end()) return;
	(*(*itt))->setDestination(Point3D( m_pitch->centerX(), goal));
	++itt;

	// defenders - inside 6 yard area.
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_defenders);

	playerPos = Point3D( m_pitch->centerX() - ((m_defenders-1)/2.0)*spacing, def);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_defenders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}

	// midfielders

	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_midfielders);
	playerPos = Point3D( m_pitch->centerX() - ((m_midfielders-1)/2.0)*spacing * (attack ? 0.9 : 1), mid);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_midfielders; count++) {
		if(itt==m_players.end()) return;
		(*(*itt))->setDestination(playerPos);
		playerPos += inc;
		++itt;
	}


	// attackers
	spacing = (m_pitch->rightBound()-m_pitch->leftBound()) / (m_attackers);
	playerPos = Point3D( m_pitch->centerX() - ((m_midfielders-1)/2.0)*spacing * (attack ? 0.9 : 1), mid);
	inc = Point3D( spacing, 0);

	for(count = 0; count<m_attackers; count++) {
		if(itt==m_players.end()) return;

		if(m_pitch->insideCircle(playerPos)) {
			(*(*itt))->setDestination(	Point3D(playerPos.x(),
							m_topHalf ? m_pitch->centerSpot().y() - m_pitch->circleRadius() :
									m_pitch->centerSpot().y() + m_pitch->circleRadius()));
		} else {
			(*(*itt))->setDestination(playerPos);
		}

		playerPos += inc;
		++itt;
	}

	// if on the attack, move closest player to the throwin spot.

	if(attack) {
		calculateClosestPlayers(pos);
		m_firstPlayer->setDestination(pos);
	}

}

Player *Team::findPlayerFromPosition(Point3D position, int direction)
{
	Player *player = 0;
	double curLength = 0;

	std::list<PlayerContainer>::iterator itt;

	// calculate the "projection cone" lines
	Point3D projectLeft = (Matrix::rollTransform(-0.42) * Player::dirVal[direction]).normalise();
	Point3D projectRight = (Matrix::rollTransform(0.42) * Player::dirVal[direction]).normalise();

	for(itt = m_players.begin(); itt!=m_players.end(); ++itt) {
		Point3D vec = (*(*itt))->position() - position;

		if(vec.liesWithinArc(projectLeft, projectRight)) {
			double len = vec.length();
			// make sure we don't pick a player exactly at "position"
			if(len < 1) continue;

			if((!player) || (len < curLength)) {
				player = (*(*itt));
				curLength = len;
			}
		}
	}

	return player;
}
