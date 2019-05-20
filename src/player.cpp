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

#include "player.h"

#include <string>
#include <iostream>
#include <cmath>

#include "resources/surfacemanager.h"
#include "matrix.h"
#include "rect.h"
#include "ball.h"
#include "team.h"
#include "pitch.h"
#include "gameengine.h"
#include "logger/logger.h"

Point3D Player::dirVal[8] = 	{  	Point3D(0,		1, 		0),
					Point3D(0.707107, 	0.707107, 	0),
					Point3D(1, 		0,		0),
					Point3D(0.707107, 	-0.707107, 	0),
					Point3D(0, 		-1, 		0),
					Point3D(-0.707107, 	-0.707107, 	0),
					Point3D(-1, 		0,		0),
					Point3D(-0.707107, 	0.707107, 	0)};


double Player::lineControl = 350;
double Player::cornerControl = 600;
double Player::shootControl = 1000;
double Player::kickHeight = 32;
double Player::moveSlowdown = 0.7;

/** Closer than this and the player starts walking */
double Player::walkDistance = 5000;
/** Further away than this and the player starts running */
double Player::runDistance = 20000;


Player::Player(Graphics *renderer, std::string skin, std::string playerMarker, Pitch *pitch, Team *team, Ball *ball, bool goalie) :
	Body(Point3D(512.0, 1024.0, 0.0), Point3D(0.0, 0.0, 0.0), Rect()),
	m_desiredPosition(Point3D(512.0, 1024.0, 0.0))
{
	m_limpSpeed = 1.5;
	m_walkSpeed = 2.6;
	m_runSpeed = 3.5;

	m_pitch = pitch;
	m_renderer = renderer;
	m_ball = ball;
	m_team = team;
	m_goalie = goalie;

	m_commitTime = 0;
	m_kickCountdown = 0;
	m_touchTimer = 0;
	m_justTurned = false;
	m_dribbleTimer = 0;

	loadSpriteSurfaces(skin, playerMarker);
	m_refCount=0;

	m_direction=0;
	m_directionCount=0;
	m_move=Stand;

	SDL_Rect sr = {-32, -54, 0, 0};
	SDL_Rect sh = {-14, -60, 0, 0};
	SDL_Rect sa = {-16, -60, 0, 0};
	m_object = new SpriteObject(0, 0, 0, position(), sr, sh, sa);
	m_renderer->addSprite(m_object);

}

Player::~Player()
{
	delete m_walk;
	delete m_run;
	delete m_stand;
	delete m_header;
	delete m_tackle;
	delete m_object;
	SurfaceManager::instance()->release(m_active);
}

void Player::update()
{
	if(m_touchTimer>0) m_touchTimer--;
	if(m_commitTime>0) m_commitTime--;
	if(m_kickCountdown>0) m_kickCountdown--;
	if(m_dribbleTimer>0) m_dribbleTimer--;
	calculateMovement();
	move();
	updateMove();
}

void Player::calculateDestintaion()
{
}

void Player::calculateMovement()
{
	if(controlState() == Full) {
		tryTrapBall();

		if(!isActive()) {
			/* This code controls the movement of players who are not on the ball when the ball
			 * is in play. Players should move towards their desired position, whilst also acting
			 * intelligently to e.g. intercept the ball.
			 */

			if(m_goalie) {
				moveGoalkeeper();
			} else {
				Point3D wantToBe = m_desiredPosition;
				Point3D curDist = position() - wantToBe;

				if(curDist.length() < m_limpSpeed*m_limpSpeed) {
					setMove(Stand);
					setDirection(closestDirection(m_ball->position()));
				} else {
					if((m_move!=Run) && (m_move!=Walk)) {
						setMove(Run);
					}

					if(curDist.length() < walkDistance) {
						setMove(Walk);
					} else if(curDist.length() > runDistance) {
						setMove(Run);
					}
					setDirection(closestDirection(wantToBe));
				}
			}
		}
	} else	if(controlState() == KickOnly) {
		setMove(Stand);
		if(!isActive()) {
			setDirection(closestDirection(m_ball->position()));
		}
	} else {
		moveToDesiredPosition();
	}
	m_justTurned = false;
}

void Player::move()
{
	accelerate(Point3D(0, 0, Pitch::gravity));
	Body::move();
	if(position().z() < 0) {
		setPosition(Point3D(position().x(), position().y(), 0.0));
	}

	if((m_move == Tackle) || (m_move == HeavyTackle) || ((m_move == Header) && (position().z()==0))) {
		applyFriction(m_pitch->friction());
		m_pitch->scratchSurface(position());
	}

	if(isActive()) {
		m_object->setOverlaySurface(m_active);
	} else {
		m_object->setOverlaySurface(0);
	}

	m_object->setPosition(position());
	m_object->setSurface(getSurface());
}

void Player::addReference() {
	m_refCount++;
}

void Player::freeReference() {
	m_refCount--;
	if(!m_refCount) delete this;
}

void Player::loadSpriteSurfaces(std::string skin, std::string playerMarker)
{
	if(m_renderer && m_renderer->screen()) {

		std::string basedir;
		if(m_goalie) {
			basedir = "graphics/" + skin + "_goalie";
		} else {
			basedir = "graphics/" + skin;
		}

		m_walk = new SpriteSequence(m_renderer->screen()->format,
								basedir+"/walking", "walking", 40, true);
		m_stand = new SpriteSequence(m_renderer->screen()->format,
								basedir+"/standing", "standing", 1, true);
		m_run = new SpriteSequence(m_renderer->screen()->format,
								basedir+"/running", "running", 40, true);
		m_tackle = new SpriteSequence(m_renderer->screen()->format,
								basedir+"/tackling", "tackling", 25, false);
		m_header = new SpriteSequence(m_renderer->screen()->format,
								basedir+"/header", "header", 20, false);
		m_active = SurfaceManager::instance()->load(m_renderer->screen()->format, playerMarker, true, false);
	} else {
		ERROR("cannot load Player Sprite surfaces, problem with renderer");
		m_walk = 0;
		m_stand = 0;
		m_run = 0;
		m_tackle = 0;
		m_header = 0;
		m_active = 0;
	}
}

void Player::setDestination(const Point3D &pos)
{
	m_desiredPosition = pos;
}

bool Player::setMove(Moves move, int variable)
{
	Point3D v;
	Player *player;

	if(committedToMove()) return false;

	if(move!=m_move) {
		switch(move) {
			case Stand:
				break;
			case Limp:
				break;
			case Walk:
				break;
			case Run:
				break;
			case Tackle:
				m_commitTime = 140;
				break;
			case HeavyTackle:
				m_commitTime = 200;
				break;
			case Header:
				m_commitTime = 200;
				break;
			case Pass:
				if(controlState() == None) return false;
				if(!isActive()) return false;
				if(!touchingBall(true)) return false;

				player = m_team->findPlayerFromPosition(position(), m_direction);

				if(player==0) {
					v = dirVal[m_direction]*m_runSpeed * 1.4;
					v.setZ(2);
				} else {
					WARN("FIXME : pass code passes to players feet where he is now, not where he is " <<
						"going to be when the ball reaches him");

					Point3D aimFor = player->position();

					v = m_ball->calculateReqVelocity(2, aimFor);

					double min = moveSpeed(Run)*1.2;
					double max = moveSpeed(Run)*3.0;

					if(v.length() < min * min) {
						v.setZ(0);
						v = v.normalise() * min;
						v.setZ(2);
					}
					if(v.length() > max * max) {
						v.setZ(0);
						v = v.normalise() * max;
						v.setZ(2);
					}
				}

				m_ball->kickBall(v, currentKickPriority()+1, this);
				m_commitTime = 50;
				break;
			case Shoot:
				if(controlState() == None) return false;
				if(!isActive()) return false;
				if(!touchingBall(true)) return false;

				v = dirVal[m_direction] * (variable / 100.0 * m_runSpeed*1.5);
				v.setZ(3);
				m_ball->kickBall(v, currentKickPriority()+1, this);
				m_commitTime = 50;
				m_kickCountdown = 50;
				break;
			case FallOver:
				WARN("FIXME : case FallOver not handles in setMove");
				break;
		}

        double k = moveSpeed(move);
		if(m_touchTimer!=0) k*= Player::moveSlowdown;

		setVelocity(dirVal[m_direction] * k);
		if(move==Header) {
			accelerate(Point3D(0,0, 2.5));
		}

		m_move = move;
		resetMove();
		return true;
	}
	return false;
}

void Player::resetMove()
{
	switch(m_move) {
		case Stand: 	m_stand->restartSequence();
				break;
		case Limp:
		case Walk: 	m_walk->restartSequence();
				break;
		case Run: 	m_run->restartSequence();
				break;
		case Tackle:
		case HeavyTackle:
				m_tackle->restartSequence();
				break;
		case Header:	m_header->restartSequence();
				break;
		case Pass:
				m_run->restartSequence();
				break;
		case Shoot:
				m_run->restartSequence();
				break;
		case FallOver:
				WARN("FIXME : case FallOver not handles in resetMove");
				break;
	}
}

void Player::updateMove()
{
	switch(m_move) {
		case Stand: 		m_stand->updateSequence();
					break;
		case Limp:
		case Walk: 		m_walk->updateSequence();
					break;
		case Run: 		m_run->updateSequence();
					break;
		case Tackle:
		case HeavyTackle:	m_tackle->updateSequence();
					break;
		case Header:		m_header->updateSequence();
					break;
		case Pass:		m_run->updateSequence();
					break;
		case Shoot:		m_run->updateSequence();
					break;
		case FallOver:
		default:
					WARN("FIXME : case " << m_move << " not handles in updateMove");
					break;
	}
}

SDL_Surface *Player::getSurface()
{

	switch(m_move) {
		case Stand: 		return m_stand->surface(m_direction);
		case Limp: 		return m_walk->surface(m_direction);
		case Walk: 		return m_walk->surface(m_direction);
		case Run: 		return m_run->surface(m_direction);
		case Tackle:
		case HeavyTackle:	return m_tackle->surface(m_direction);
		case Header:		return m_header->surface(m_direction);
		case Pass:		return m_run->surface(m_direction);
		case Shoot:		return m_run->surface(m_direction);
		case FallOver: // TODO
		default: 		return m_stand->surface(m_direction);
	}
}

bool Player::inPosition()
{
	Point3D curDist = position();
	curDist -= m_desiredPosition;

	if(curDist.length() < m_limpSpeed*m_limpSpeed) return true;
	return false;
}

void Player::setDirection(int direction)
{
	if((direction < 0) || (direction > 7)) {
		WARN("Player::setDirection passed out of range direction " <<
			direction << " - ignoring");
		return;
	}
	double k;

	//
	// If we are tackling, we can't do anything else until we've finished sliding!
	//
	if(!committedToMove()) {
		if(direction!=m_direction) {
			m_justTurned=true;
		}
		m_direction=direction;

		k = moveSpeed(m_move);
		if(m_touchTimer!=0) k*= Player::moveSlowdown;

		Point3D newVelocity = dirVal[m_direction]*k;

		// Player "guidance" so that it is easy to connect with the ball. If the player is slightly
		// off line, he will be moved on line, so you don't have to press as many diagonals to hit
		// the ball. Subtle, but effective.
		if((controlState()==Full) && (isActive())) {
			if( (position() + newVelocity - m_ball->position()).length() <
				(position() - m_ball->position()).length() ) {
				Point3D left = Matrix::rollTransform(-0.174539252) * dirVal[m_direction];
				Point3D right= Matrix::rollTransform(0.174539252) * dirVal[m_direction];

				if( (m_ball->position() - position()).liesWithinArc(left, right)) {
					newVelocity = (m_ball->position() -
						       		position()).setZ(0).normalise() * k;
				}
			}
		}

		setVelocity(newVelocity);
	}
}

void Player::setActive(bool state)
{
	m_isActive = state;
}

double Player::distanceFrom(const Point3D &pos)
{
	Point3D dist = position() - pos;
	return dist.length();
}


bool Player::isActive() {
	return m_isActive;
}

void Player::setControlState(ControlState state)
{
	m_controlState = state;

	if(state == None) {
		setActive(false);
	}
}

Player::ControlState Player::controlState()
{
	return m_controlState;
}


bool Player::justTurned()
{
	return m_justTurned;
}

void Player::setDesiredPosition(const Point3D &pos)
{
	m_desiredPosition = pos;
}


int Player::closestDirection(const Point3D &pos)
{
	Point3D newDist;
	int dir = 0;
	double currentLength = (position() - pos).length();

	m_directionCount=0;
	for(int count=0; count<8; count++) {
		newDist = position() + (dirVal[count] * 0.01) - pos;
		if(newDist.length() < currentLength) {
			dir =  count;
			currentLength = newDist.length();
		}
	}

	return dir;
}

bool Player::committedToMove()
{
	//
	// Shooting/passing takes time!
	// If we are tackling, we can't do anything else until we've finished sliding!
	//

	return m_commitTime>0;
}

bool Player::applyAftertouch(int direction)
{
	if(!m_kickCountdown) return false;


	// Find the unit vector for the ball
	Point3D ubv = m_ball->velocity();
	ubv.setZ(0);

	if(ubv.length() <= 0.00000001) {
		m_kickCountdown = 0;
		return false;   // an upwards moving ball does not have aftertouch applied.
	}
	ubv *= 1.0 / sqrt(ubv.length());

	// Determine unit vector perpendicular to the ball direction.
	ubv = Matrix::rollTransform(1.570796327) * ubv;


	// Rotate direction vector by so many radians around the direction vector.
	Point3D udv = dirVal[direction];
	udv = Matrix::arbitraryTransform(ubv, 1.2) * udv;

	m_ball->accelerate(udv * 0.07);

	return true;
}

bool Player::touchingBall(bool shooting)
{
	if((controlState() == Full) || (controlState() == KickOnly)) {
		double len = (m_ball->position() - position()).length();
		if(m_ball->position().z() < kickHeight) {
			if( (justTurned() && len < cornerControl) ||
			    (len < lineControl) || (shooting && len < shootControl)) {
				return true;
			}
		}
	}
	return false;
}

int Player::currentKickPriority()
{
	int priority = 2;

	if(m_touchTimer > 0) priority--;
	if(m_move == Tackle) priority++;
	if(m_move == HeavyTackle) priority += 2;
	if(m_move == Stand) priority -=1;

	if((m_goalie) && (m_touchTimer==0)) {
		priority = 5;
	}

	return priority;
}

void Player::tryTrapBall()
{
	/* This code lets a player trap the ball. */
	Point3D groundVel = velocity();
	groundVel.setZ(0);
	if(groundVel.length()>0) {
		if(fabs(m_ball->position().z() - position().z()) < kickHeight) {
			if(touchingBall(false)) {
				if((m_move!=Shoot) && (m_move!=Pass)) {
					Point3D reqvel;
					if((m_move==Limp) || (m_move==Walk) || (m_move==Run)) {
						Point3D dest = position() + (velocity()*10) + ((groundVel.normalise()) * 32);
						reqvel = dest - m_ball->position();
						reqvel *= 0.07;
						if(m_ball->position().z()==0) {
							reqvel.setZ(0.1 - Pitch::gravity);
						} else {
							reqvel.setZ(m_ball->velocity().z());
						}
					} else {
						reqvel = velocity() * 1.3;
					}

					if( sqrt((reqvel - m_ball->velocity()).length()) / sqrt(m_ball->velocity().length()) > 0.05) {
						m_ball->kickBall(reqvel,
							currentKickPriority(),
							this);
						m_touchTimer = 100;
					}
				}
			}
		}
	}
}

bool Player::isGoalie()
{
	return m_goalie;
}

double Player::moveSpeed(Moves move)
{
	switch(move) {
		case Stand 	:	return 0.0;
		case Limp	:	return m_limpSpeed;
		case Walk	:	return m_walkSpeed;
		case Run	:	return m_runSpeed;
		case Tackle	:	return m_runSpeed * 2;
		case HeavyTackle:	return m_runSpeed * 3;
		case Pass	:	return m_runSpeed;
		case Shoot	:	return m_runSpeed;
		case Header	:	return m_runSpeed * 2;
		case FallOver: // TODO
		default		:	WARN("moveSpeed() asked for unknown move");
					return m_runSpeed;
	}
}

void Player::turnTo(int direction)
{
	setDirection(direction);
}

Team *Player::team()
{
	return m_team;
}

bool Player::hasBall()
{
	if(!isActive()) return false;
	if(GameEngine::lastPlayerTouch() != this) return false;
	if(m_touchTimer == 0) return false;

	return true;
}

void Player::moveToDesiredPosition()
{
	/* This code moves players to where they are supposed to be when the ball is not in play,
	 *  and we are not waiting for a set piece to take place. No ball interaction takes place.
	 **/
	Point3D curDist = position();
	Point3D newDist = position();

	curDist -= m_desiredPosition;
	double currentLength = curDist.length();

	if(currentLength < m_limpSpeed*m_limpSpeed) {
		setMove(Stand);
		setDirection(closestDirection(m_ball->position()));
	} else {
		newDist += dirVal[m_direction] * m_limpSpeed;
		newDist -= m_desiredPosition;

		int dir = m_direction;
		if((++m_directionCount > 100) || (newDist.length() >= currentLength)) {
			dir = closestDirection(m_desiredPosition);
			m_directionCount = 0;
		}

		if(currentLength < m_walkSpeed*m_walkSpeed) {
			setMove(Limp);
		} else if(currentLength < m_runSpeed*m_runSpeed) {
			setMove(Walk);
		} else {
			setMove(Run);
		}
		setDirection(dir);
	}
}

void Player::moveGoalkeeper()
{
	Point3D wantToBe = m_desiredPosition;

	if(m_pitch->insideTheArea(m_ball->position(), m_pitch->inTopHalf(m_desiredPosition))) {
		wantToBe = m_ball->position();
	} else {
		Point3D vector = m_ball->position() - m_desiredPosition;
		double k = sqrt(vector.length());

		if(k!=0) {
			vector *= 250/k;
			k = vector.y();
			if(k<0) k = -k;
			if(k>10) {
				vector = Point3D(vector.x(),
						  vector.y() / 10,
						  vector.z());
			}
			wantToBe = m_desiredPosition+vector;
		}
	}

	Point3D curDist = position() - wantToBe;

	if(curDist.length() < m_limpSpeed*m_limpSpeed) {
		setMove(Stand);
		setDirection(closestDirection(m_ball->position()));
	} else {
		if((m_move!=Run) && (m_move!=Walk)) {
			setMove(Run);
		}

		if(curDist.length() < walkDistance) {
			setMove(Walk);
		} else if(curDist.length() > runDistance) {
			setMove(Run);
		}
		setDirection(closestDirection(wantToBe));
	}
}
