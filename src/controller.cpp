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

#include "controller.h"

#include <math.h>
#include <iostream>

#include "player.h"
#include "gameengine.h"

Controller::Controller(Team *team, Team *opponents, SDLKey up, SDLKey down, SDLKey left, SDLKey right, SDLKey shoot, SDLKey pass, bool isCPU, Ball *ball, Pitch *pitch)
{
	m_team = team;
	m_oppTeam = opponents;
	m_up = up;
	m_down = down;
	m_left = left;
	m_right = right;
	m_shoot = shoot;
	m_pass = pass;
	m_shootCount = 0;
	m_canKick = true;
	m_lastPlayer = 0;
	m_isCPU = isCPU;
	m_ball = ball;
	m_pitch = pitch;
	m_lastPass = 0;
}

Controller::~Controller()
{
}

void Controller::updateController(Uint8 *keys)
{
	if(m_isCPU) {
		updateComputer();
		return;
	}
	
	Player *player;

	int x=0, y=0;
	int dir = -1;

	player = m_team->activePlayer();

	if(!player) return;
	
	if(keys[m_up]) 	y--;
	if(keys[m_down]) y++;
	if(keys[m_left]) x--;
	if(keys[m_right]) x++;

	// calculate move
					
	if(x==-1) {
		if(y==-1) {
			dir = 5;
		} else if(y==1) {
			dir = 7;
		} else {
			dir = 6;
		}
	} else if(x==1) {
		if(y==-1) {
			dir = 3;
		} else if(y==1) {
			dir = 1;
		} else {
			dir = 2;
		}
	} else {
		if(y==-1) {
			dir = 4;
		} else if(y==1) {
			dir = 0;
		} else {
			dir = -1;
		}
	}
	
	//
	// Apply aftertouch if necessary.
	//
	if(m_lastPlayer!=0) {
		if(!m_lastPlayer->applyAftertouch(dir)) {
			m_lastPlayer = 0;
		}
	}
	
	if(dir!=-1) {
		player->setMove(Player::Run);
		player->turnTo(dir);
	} else {
		player->setMove(Player::Stand);
	}
	if(m_canKick) {	
		if(keys[m_shoot]) {
			if((!player->touchingBall(true)) && (m_shootCount==0)) {
				if(m_ball->position().z() > Player::kickHeight) {
					player->setMove(Player::Header);
				} else {
					player->setMove(Player::HeavyTackle);
				}
				m_canKick = false;
			} else {
				m_shootCount++;
				if(m_shootCount==10) {
					if(!player->setMove(Player::Shoot, 100)) {
						player->setMove(Player::HeavyTackle);
					} else {
						m_lastPlayer = player;
					}
					m_shootCount = 0;
					m_canKick = false;
				}
			}
		} else {
			if(m_shootCount > 0) {
				if(!player->setMove(Player::Shoot, m_shootCount*10)) {
					player->setMove(Player::HeavyTackle);
				} else {
					m_lastPlayer = player;
				}
				m_canKick = false;
			}
			m_shootCount = 0;			
		}
		if(keys[m_pass]) {
			if(!player->setMove(Player::Pass)) {
				if(m_ball->position().z() > Player::kickHeight) {
					player->setMove(Player::Header);
				} else {
					player->setMove(Player::Tackle);
				}
			}
			m_canKick = false;
		}
	} else if(!(keys[m_shoot] | keys[m_pass])) {
		m_canKick = true;
	}	
}


void Controller::updateComputer()
{
	m_lastPass++;
	
	Player *player = m_team->activePlayer();
	if(player==0) return;
	
	Point3D goalCenter(m_pitch->centerX(),
				m_team->topHalf() ? m_pitch->bottomBound() : m_pitch->topBound());
	
	/** Closest Direction to the opponents goal mouth. */
	int dirGoIn = player->closestDirection(Point3D(m_pitch->centerX(), 
				m_team->topHalf() ? m_pitch->bottomBound() : m_pitch->topBound()));
	
	/** Closest direction to reach the balls position */
	int dirBall = player->closestDirection(m_ball->position());

	if(player->controlState() == Player::KickOnly) {
		if(player->touchingBall(true)) {
			player->turnTo(player->closestDirection(m_pitch->centerSpot()));
			player->setMove(Player::Shoot, 100);
		}
	} else 	if(GameEngine::lastTeamTouch() == m_team) {
		// on attack - we have the ball.	
		
		if(player->touchingBall(false)) {
			if( (goalCenter - m_ball->position()).length() < 640000) {
				player->turnTo(player->closestDirection(goalCenter));
				player->setMove(Player::Shoot, 100);
			} else {
				int bestDirection = calculateBestPass(player);
	
				if((m_lastPass > 200) && (bestDirection!=-1) && (calculateClosestOpponent(player) < 40000)) {
					player->turnTo(bestDirection);
					player->setMove(Player::Pass);
					m_lastPass = 0;
				} else {
					/* Move towards goal */
					player->turnTo(player->closestDirection(goalCenter));
					player->setMove(Player::Run);
				}
			}
		} else {
			player->turnTo(dirBall);
			player->setMove(Player::Run);
		}
	} else {
		// on defense - do not have the ball
		if(player->touchingBall(false)) {
			player->turnTo(dirBall);
			player->setMove(Player::Run);
		} else {
			/* Move towards ball to claim it if it's not moving*/
			if(m_ball->velocity().setZ(0).length() < 5) {
				player->turnTo(dirBall);
				player->setMove(Player::Run);
			} else {
				// check our position relative to the ball.
				int ballMoveDir = player->closestDirection((player->position() + m_ball->velocity()).setZ(0));
				ballMoveDir = (ballMoveDir + 4)%8;
				ballMoveDir -= dirBall;
				if(ballMoveDir < -4) ballMoveDir += 8;
				if(ballMoveDir > 4) ballMoveDir -= 8;
				
				// if the ball is moving towards us...
				if(( abs(ballMoveDir) < 2) &&
						((m_ball->position() - player->position()).length() < 40000)) {
					player->turnTo(dirBall);

					// ... either tackle or run, depending on how close the opponent is.
					if(calculateClosestOpponent(player) > 
							(player->position() - m_ball->position()).length()) {
						player->setMove(Player::Run);
					} else {
						player->setMove(Player::HeavyTackle);
					}
				} else {
					// If the ball isn't moving towards us, we run to intercept it.
					dirGoIn = player->closestDirection(m_ball->position() + (m_ball->velocity().setZ(0).normalise() * 200));
					player->turnTo(dirGoIn);
					player->setMove(Player::Run);
				}
			}
		}
	}
}

int Controller::calculateBestPass(Player *player) {
	int bestDirection = -1;
	double bestLen = -1;
	Player *us, *them;
	Point3D goalCenter(m_pitch->centerX(),
				m_team->topHalf() ? m_pitch->bottomBound() : m_pitch->topBound());
	
	for(int count=0; count<8; count++) {
		us = m_team->findPlayerFromPosition(player->position(), count);
		them = m_oppTeam->findPlayerFromPosition(player->position(), count);

		if(!us) continue;

		if(them) {
			Point3D v1 = (us->position() - m_ball->position()).normalise();
			Point3D v2 = (them->position() - m_ball->position()).normalise();
			if(acos(v1.dotProduct(v2)) < 0.25) continue;
		}
				
		double len = (us->position() - goalCenter).length();
		if((bestDirection == -1) || (bestLen > len)) {
			bestDirection = count;
			bestLen = len;
		}
	}

	return bestDirection;
}

double Controller::calculateClosestOpponent(Player *player)
{	
	Player *p = m_oppTeam->calculateClosestPlayers(player->position());
	
	if(p) {
		return (p->position() - player->position()).length();
	}
	return -1.0;
}

SDLKey Controller::keyUp()
{
	return m_up;
}

SDLKey Controller::keyDown()
{
	return m_down;
}

SDLKey Controller::keyLeft()	
{
	return m_left;
}

SDLKey Controller::keyRight()
{
	return m_right;
}

SDLKey Controller::keyPass()
{
	return m_pass;
}

SDLKey Controller::keyShoot()	
{
	return m_shoot;
}

void Controller::setKeyUp(SDLKey key)
{
	m_up = key;
}

void Controller::setKeyDown(SDLKey key)
{
	m_down = key;
}

void Controller::setKeyLeft(SDLKey key)
{
	m_left = key;
}

void Controller::setKeyRight(SDLKey key)
{
	m_right = key;
}

void Controller::setKeyPass(SDLKey key)
{
	m_pass = key;
}

void Controller::setKeyShoot(SDLKey key)
{
	m_shoot = key;
}

void Controller::setCPU(bool cpu)
{
	m_isCPU = cpu;
}
