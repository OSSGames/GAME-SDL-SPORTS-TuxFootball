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

#include "ball.h"

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>

#include "spriteobject.h"
#include "graphics.h"
#include "segment.h"
#include "pitch.h"
#include "resources/surfacemanager.h"
#include "resources/soundmanager.h"
#include "gameengine.h"
#include "rect.h"
#include "point3d.h"
#include "logger/logger.h"

double Ball::bounceFactor = 0.6;

Ball::Ball(Graphics *renderer, Pitch *pitch) :
	Body(Point3D(0, 0, 300), Point3D(0, 0, 0), Rect()),
	m_pitch(pitch), m_renderer(renderer), m_lastPosition(position())
{

	if(m_renderer && m_renderer->screen()) {
		m_football = SurfaceManager::instance()->load(m_renderer->screen()->format, "graphics/football.png", true, false);
		m_shadow = SurfaceManager::instance()->load(m_renderer->screen()->format, "graphics/footballshadow.png", true, false);
	} else {
		ERROR("Screen not defined - cannot initialise ball graphics");
		m_football = 0;
		m_shadow = 0;
	}

	// Create the sprite for the ball including it's shadow (and no overlay for the ball)
	SDL_Rect sr = {-4, -9, 0, 0};
	SDL_Rect sh = {-1, -7, 0, 0};
	SDL_Rect sa;
	m_object = new SpriteObject(m_football, m_shadow, 0,  position(), sr, sh, sa);
	m_renderer->addSprite(m_object);

	m_kickVel = Point3D();
	m_kickPriority = -1;
	m_kickBy = 0;

	m_bounce = SoundManager::instance()->load("sound/bounce.wav");
	m_kick = SoundManager::instance()->load("sound/kick.wav");
	m_rebound = SoundManager::instance()->load("sound/bounce.wav");
}

Ball::~Ball()
{
	if(m_football) SurfaceManager::instance()->release(m_football);
	if(m_shadow) SurfaceManager::instance()->release(m_shadow);
	if(m_object) delete m_object;
	if(m_bounce) SoundManager::instance()->release(m_bounce);
	if(m_kick) SoundManager::instance()->release(m_kick);
	if(m_rebound) SoundManager::instance()->release(m_rebound);
}

void Ball::move()
{
	m_lastPosition = position();
	accelerate(Point3D(0, 0, Pitch::gravity));

	Point3D before = position();

	Body::move();

	if(position().z() < 0) {
		setPosition(Point3D(	position().x(),
					position().y(),
					position().z()*-bounceFactor));
		setVelocity(Point3D(	velocity().x()*m_pitch->friction(),
					velocity().y()*m_pitch->friction(),
					velocity().z()*-bounceFactor));
		if(velocity().z() > Pitch::gravity * -2) {
			Mix_PlayChannel(-1, m_bounce, 0);
		}
	}

	Point3D top, bot;

	bool goalPostHit = false;

	//
	// Goal Post collision detection code.
	//

	top = bot = m_pitch->goalPost(true, true);
	top.setZ(m_pitch->goalHeight());
	if(m_pitch->closestPointLineSegments(	Segment(before, position()),
						Segment(bot, top)) < 256) goalPostHit = true;

	top = bot = m_pitch->goalPost(true, false);
	top.setZ(m_pitch->goalHeight());
	if(m_pitch->closestPointLineSegments(	Segment(before, position()),
						Segment(bot, top)) < 256) goalPostHit = true;

	top = bot = m_pitch->goalPost(false, true);
	top.setZ(m_pitch->goalHeight());
	if(m_pitch->closestPointLineSegments(	Segment(before, position()),
						Segment(bot, top)) < 256) goalPostHit = true;

	top = bot = m_pitch->goalPost(false, false);
	top.setZ(m_pitch->goalHeight());
	if(m_pitch->closestPointLineSegments(	Segment(before, position()),
						Segment(bot, top)) < 256) goalPostHit = true;

	//
	// Goal Cross bar collision detection code.
	//

	top = m_pitch->goalPost(false, true);
	bot = m_pitch->goalPost(true, true);
	top.setZ(m_pitch->goalHeight());
	bot.setZ(m_pitch->goalHeight());
	if(m_pitch->closestPointLineSegments(	Segment(before, position()),
						Segment(bot, top)) < 256) goalPostHit = true;

	top = m_pitch->goalPost(false, false);
	bot = m_pitch->goalPost(true, false);
	top.setZ(m_pitch->goalHeight());
	bot.setZ(m_pitch->goalHeight());
	if(m_pitch->closestPointLineSegments(	Segment(before, position()),
						Segment(bot, top)) < 256) goalPostHit = true;

	if(goalPostHit) {
		Mix_PlayChannel(-1, m_rebound, 0);
		setVelocity(velocity() * -1);
		Body::move();
		setVelocity(velocity() * 0.7);
	}

	m_object->setPosition(position());
}

Point3D Ball::lastPosition()
{
	return m_lastPosition;
}

void Ball::kickBall(const Point3D &vel, int priority, Player *kickBy)
{
	if(m_kickPriority < priority) {
		m_kickVel = vel;
		m_kickPriority = priority;
		m_kickBy = kickBy;
	} else if(m_kickPriority == priority) {
		/* 50/50 chance of which kick of the same priority actually happens.
		 * This is not strictly mathematically correct - consider if three
		 * kicks are applied to the ball simultaneously. However, in the game, it's 
		 * unlikely that this will ever occur, or that anyone will ever care. */
		if(rand()%2) {
			m_kickVel = vel;
			m_kickPriority = priority;
			m_kickBy = kickBy;
		}
	}
}

void Ball::applyKick()
{
	if(m_kickPriority!=-1) {
		setVelocity(m_kickVel);
		m_kickPriority = -1;
		GameEngine::setLastTouch(m_kickBy);
		m_kickBy = 0;
		Mix_PlayChannel(-1, m_kick, 0);
	}
}

Point3D Ball::calculateReqVelocity(double z, Point3D end)
{
	// calculate number of bounces until ball starts rolling.
	//
	// Calculation worked out with the following steps :
	// 1. Calculate number of bounces until the ball is rolling.
	// 2. Calculate the distance equation for how far the ball travels during these bounces
	// 3. Calculate the distance travelled before the ball stops rolling.
	//
	// Steps 2 & 3 rely on geometric series.

	double s = sqrt((end-position()).length());
	double f = m_pitch->friction();
	double h = m_pitch->friction();	// this is the friction when the ball is rolling rather than bouncing.
	double g = -Pitch::gravity;
	double b = bounceFactor;

	int n = (int)(log (g/z) / log(b));
	if(z<=0) {
		n = 0;
	}

	// double u = (s*g*(1-f*b)) / (2*z*(1-pow(f, n-1)*pow(b, n-1)));
	double u = s / ( (2*z/g)*(1-pow(f, n-1)*pow(b, n-1))/(1-f*b) + (pow(h, n)/(1-h)));

	Point3D ret = ((end-position()).normalise())*u;
	ret.setZ(z);

	return ret;
}
