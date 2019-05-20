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

#ifndef TUXFOOTBALL_PLAYER
#define TUXFOOTBALL_PLAYER

#include "SDL.h"
#include "body.h"
#include "spritesequence.h"
#include "spriteobject.h"
#include "point3d.h"
#include "graphics.h"

#include <string>

class Ball;
class Pitch;
class Team;
class GameEngine;

/** Encapsulates a football player, and all associated logic. */
class Player : public Body {
public:
	static Point3D dirVal[8];
	static double kickHeight;
	enum Moves {Stand, Limp, Walk, Run, Tackle, HeavyTackle, Pass, Shoot, Header, FallOver};
	enum ControlState { Full, KickOnly, None };
	
	Player(Graphics *renderer, std::string skin, std::string playerMarker, Pitch *pitch, Team *team, Ball *ball, bool goalie);
	~Player();
	
	void update();
	void calculateDestintaion();
	void calculateMovement();
	void move();
	void addReference();
	void freeReference();
	void setDestination(const Point3D &pos);
	bool inPosition();
	double distanceFrom(const Point3D &pos);
	void setActive(bool state);
	bool isActive();
	bool isGoalie();

	/** Returns true if the player is currently in posetion of the ball, false otherwise */
	bool hasBall();

	/** Returns the speed of a given move. Eg. running is faster than walking, which is faster than limping. */
	double moveSpeed(Moves move);

	void setControlState(ControlState state);

	/** Returns the control state for the player - either None, KickOnly or Full.
	 * This determines to what extent the player can be controlled by the controller. */
	ControlState controlState();

	/** Sets the players current move. Returns true on success, false if the
	 * player cannot do this because he is e.g. committed to another move.
	 * variable is used for some moves which require an attribute, such as shoot */
	bool setMove(Moves move, int variable=0);

	/** Turns the player to the specified direction. The player is at liberty to do this intelliegently,
	 * such as keeping hold of the ball and directing it in the new direction. */
	void turnTo(int direction);

	/** Tells the player the area on the pitch he should mooch about by. The player
	 * may move to intercept the ball, etc. though. */
	void setDesiredPosition(const Point3D &pos);

	/** Returns true if the player is committed to the move he is currently performing -
	 * he cannot start another move until this one has completed. Used for tackles,
	 * shooting, etc.*/
	bool committedToMove();

	/** Attempt to apply aftertouch to the ball, in the given direction. Returns true
	 * if aftertouch has been applied, false if the player is not in a position to
	 * apply aftertouch e.g. once the aftertouch timer has expired. */
	bool applyAftertouch(int direction);
	
	/** returns true if the player is in a position where he can manipulate the ball.
	 * If we pass "shooting" as true, we are asking if the player can kick the ball
	 * with either pass or shoot from where he is currently standing */
	bool touchingBall(bool shooting);

	/** Returns the direction which will move the player the most towards the point
	 * specified. */
	int closestDirection(const Point3D &pos);

	/** Return the team that this player belongs to */
	Team *team();
private:
	Point3D m_desiredPosition;
	SpriteSequence *m_walk;
	SpriteSequence *m_stand;
	SpriteSequence *m_run;
	SpriteSequence *m_tackle;
	SpriteSequence *m_header;
	int m_refCount;
	int m_direction;
	int m_directionCount;

	int m_commitTime;
	int m_kickCountdown;
	bool m_justTurned;
	int m_touchTimer;
	int m_dribbleTimer;
	
	Moves m_move;
	double m_limpSpeed;
	double m_walkSpeed;
	double m_runSpeed;
	static double standThreshold;
	static double lineControl;
	static double cornerControl;
	static double shootControl;
	static double walkDistance;
	static double runDistance;
	static double moveSlowdown;
	SDL_Surface *m_active;
	SpriteObject *m_object;
	bool m_isActive;
	ControlState m_controlState;
	Graphics *m_renderer;
	Ball *m_ball;
	Team *m_team;
	Pitch *m_pitch;
	bool m_goalie;
private: // methods
	/** Loads sprites for the player. This is done via a seperate surface manager, so that multiple players
	 * make use of the same sprites rather than wasting hideous amounts of memory. */
	void setDirection(int direction);
	void loadSpriteSurfaces(std::string skin, std::string playerMarker);
	void resetMove();
	void updateMove();
	SDL_Surface *getSurface();
	bool justTurned();
	int currentKickPriority();
	void tryTrapBall();
	void moveToDesiredPosition();
	void moveGoalkeeper();
};

class PlayerContainer {
public:
	PlayerContainer()
	{
		m_player = 0;
	}

	PlayerContainer(const PlayerContainer &cont) 
	{
		m_player = cont.player();
		m_player->addReference();
	}

	PlayerContainer(Player *player) {
		m_player = player;
		m_player->addReference();
	}
	
	~PlayerContainer() {
		if(m_player!= 0) {
			m_player->freeReference();
		}
	}

	PlayerContainer &operator=(PlayerContainer &cont) {
		m_player = *cont;
		if(m_player!=0) {
			m_player->addReference();
		}
		return *this;
	}

	Player *operator*() const
	{	
		return m_player;
	}

	Player *player() const
	{
		return m_player;
	}
private:
	Player *m_player;
};

#endif /* TUXFOOTBALL_PLAYER */
