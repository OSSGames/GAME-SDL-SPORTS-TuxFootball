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

#include <iostream>

#ifdef HAVE_GETTEXT
#include "gettext.h"
#define _(String) gettext (String)
#endif

#include "gameengine.h"
#include "resources/surfacemanager.h"
#include "resources/fontmanager.h"
#include "resources/soundmanager.h"
#include "Font.hpp"
#include "controller.h"

#include "menu/videosettingsmenu.h"
#include "menu/redefinekeysmenu.h"
#include "menu/mainmenu.h"
#include "menu/elements/menuoptionlist.h"
#include "menu/elements/menubutton.h"
#include "menu/elements/menukeyselect.h"
#include "menu/elements/menulabel.h"

#include "gamestates/mainmenustate.h"
#include "gamestates/redefinekeysstate.h"
#include "gamestates/matchstartstate.h"
#include "gamestates/nationalanthemstate.h"
#include "gamestates/firsthalfstate.h"
#include "gamestates/halftimestate.h"
#include "gamestates/secondhalfstate.h"
#include "gamestates/fulltimestate.h"
#include "gamestates/extratimestate.h"
#include "gamestates/extratimefirsthalfstate.h"
#include "gamestates/extratimehalftimestate.h"
#include "gamestates/extratimesecondhalfstate.h"
#include "gamestates/penaltyshootoutstate.h"
#include "gamestates/matchabortedstate.h"
#include "gamestates/matchfinishedstate.h"
#include "gamestates/videomenustate.h"
#include "const.h"
#include "logger/logger.h"

Player *GameEngine::s_lastTouch=0;

void GameEngine::setLastTouch(Player *player)
{
	s_lastTouch = player;
}

Player *GameEngine::lastPlayerTouch()
{
	return s_lastTouch;
}

Team *GameEngine::lastTeamTouch()
{
	if(s_lastTouch == 0) return 0;
	return s_lastTouch->team();
}

Team *GameEngine::otherTeam(Team *team)
{
	if(team == m_awayTeam) return m_homeTeam;
	return m_awayTeam;
}


GameEngine::GameEngine(bool fullscreen) :
	m_frameSpeed(10),
	m_fullscreen(fullscreen)
{

	// Start audio.
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		WARN("audio failed to start : " << Mix_GetError());
		m_audioOpen = false;
	} else {
		m_audioOpen = true;
	}

	m_sounds.push_back(SoundManager::instance()->load("sound/refwhistlelong.wav"));
	m_sounds.push_back(SoundManager::instance()->load("sound/refwhistleshort.wav"));
	m_sounds.push_back(SoundManager::instance()->load("sound/refwhistletwice.wav"));
	m_sounds.push_back(SoundManager::instance()->load("sound/refwhistleend.wav"));
	m_sounds.push_back(SoundManager::instance()->load("sound/crowdcheer.wav"));
	m_sounds.push_back(SoundManager::instance()->load("sound/crowdboo.wav"));
	m_sounds.push_back(SoundManager::instance()->load("sound/crowdooh.wav"));

	m_quitKey = SDLK_ESCAPE;

	m_resX = 800;
	m_resY = 600;

	uint sdl_flags = SDL_SWSURFACE;
	if (m_fullscreen) sdl_flags |= SDL_FULLSCREEN;
	m_screen = SDL_SetVideoMode(m_resX, m_resY, 32, sdl_flags);
#ifdef HAVE_CONFIG_H
	SDL_WM_SetCaption(PACKAGE_STRING, NULL);
#else
	SDL_WM_SetCaption("tuxfootball", NULL);
#endif
	if(m_screen == 0) {
		m_finished = true;
		ERROR("Unable to set " << m_resX << "x" << m_resY << " video: " << SDL_GetError());
		m_pitch = 0;
		m_ball = 0;
		m_homeTeam = 0;
		m_awayTeam = 0;
		m_renderer = 0;
		m_nameFont = 0;
		m_scoreFont = 0;
	} else {
		// Load Fonts
		m_nameFont = FontManager::instance()->load(m_screen->format, "graphics/font_yellow.png", false, true);
		m_scoreFont = FontManager::instance()->load(m_screen->format, "graphics/font_white.png", false, true);

		// Write out text to show our starting progress
		m_scoreFont->write(m_screen, _("Starting Renderer"), 10, 10);
		SDL_Flip(m_screen);
		m_renderer = new Graphics(m_screen);

		m_scoreFont->write(m_screen, _("Loading Pitch"), 10, 40);
		SDL_Flip(m_screen);
		m_pitch = new Pitch(m_renderer);

		m_scoreFont->write(m_screen, _("Loading Ball"), 10, 70);
		SDL_Flip(m_screen);
		m_ball = new Ball(m_renderer, m_pitch);

		m_scoreFont->write(m_screen, _("Loading Team 1"), 10, 100);
		SDL_Flip(m_screen);
		m_homeTeam = new Team(this, "Blue Utd.", "BLU", "team1", "graphics/homeplayermarker.png", m_pitch, m_ball, true);

		m_scoreFont->write(m_screen, _("Loading Team 2"), 10, 130);
		SDL_Flip(m_screen);
		m_awayTeam = new Team(this, "Red City", "RED", "team2", "graphics/awayplayermarker.png", m_pitch, m_ball, false);
	}

	SDL_Rect r = {(Sint16)(m_resX/2), (Sint16)(m_resY/2), (Uint16)(m_pitch->width()-(m_resX)), (Uint16)(m_pitch->height()-(m_resY))};
	m_camera.setBoundingRect(r);
	r.x = 0;
	r.y = 0;
	r.w = m_resX;
	r.h = m_resY;
	m_camera.setScreenRect(r);

	m_homeController = new Controller(m_homeTeam, m_awayTeam, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_LSHIFT, SDLK_LCTRL, false, m_ball, m_pitch);
	m_awayController = new Controller(m_awayTeam, m_homeTeam, SDLK_k, SDLK_j, SDLK_h, SDLK_l, SDLK_f, SDLK_g, true, m_ball, m_pitch);

	m_halfLength = 6000;

	m_displayFPS = false;
	m_framesPerSecond = 60;

	m_gameStates.push_back( new MainMenuState(*this) );
	m_gameStates.push_back( new RedefineKeysState(*this) );
	m_gameStates.push_back( new VideoMenuState(*this) );
	m_gameStates.push_back( new MatchStartState(*this) );
	m_gameStates.push_back( new NationalAnthemState(*this) );
	m_gameStates.push_back( new FirstHalfState(*this) );
	m_gameStates.push_back( new HalfTimeState(*this) );
	m_gameStates.push_back( new SecondHalfState(*this) );
	m_gameStates.push_back( new FullTimeState(*this) );
	m_gameStates.push_back( new ExtraTimeState(*this) );
	m_gameStates.push_back( new ExtraTimeFirstHalfState(*this) );
	m_gameStates.push_back( new ExtraTimeHalfTimeState(*this) );
	m_gameStates.push_back( new ExtraTimeSecondHalfState(*this) );
	m_gameStates.push_back( new PenaltyShootoutState(*this) );
	m_gameStates.push_back( new MatchAbortedState(*this) );
	m_gameStates.push_back( new MatchFinishedState(*this) );

}

GameEngine::~GameEngine()
{
	if(m_pitch) delete m_pitch;
	if(m_homeTeam) delete m_homeTeam;
	if(m_awayTeam) delete m_awayTeam;

	if(m_homeController) delete m_homeController;
	if(m_awayController) delete m_awayController;
	if(m_renderer) delete m_renderer;

	if(m_nameFont) FontManager::instance()->release(m_nameFont);
	if(m_scoreFont) FontManager::instance()->release(m_scoreFont);

	std::vector<Mix_Chunk *>::iterator it = m_sounds.begin();
	while(it != m_sounds.end())
	{
		if(*it) SoundManager::instance()->release((*it));
		++it;
	}

	// Shutdown Audio.
	Mix_CloseAudio();

	std::vector<StateBase *>::iterator itt = m_gameStates.begin();
	while(itt != m_gameStates.end()) {
		delete(*itt);
		++itt;
	}
}

void GameEngine::gameLoop()
{
	static double totalFPSTime = 0;
	m_music = 0;

	int nextTime = SDL_GetTicks();
	m_camera.setPosition(Point3D(m_pitch->width()/2, m_pitch->height()-m_screen->h));

	m_finished = false;
	// Let's start with the title screen
	setState(TitleScreen);
	initialiseMatch();

	do {
		int startTime = SDL_GetTicks();
		int currentTime;

		// Make sure we wait for the next frame if we are going too fast...
		while( (currentTime = SDL_GetTicks()) < nextTime ) {
			SDL_Delay(1);
		}

		// And make sure we catch up if we are going too slow.
		while(nextTime < currentTime) {
			iterateEngine();
			nextTime += m_frameSpeed;
			if(timerState() == Start) {
				m_timer++;
			}
		}

		drawFrame();
		startTime = SDL_GetTicks() - startTime;
		totalFPSTime = ((totalFPSTime * 9.0) + startTime)/10.0;
		if((int)totalFPSTime != 0) {
			m_framesPerSecond = (int)(1000.0 / totalFPSTime);
		}
	} while(!finished());
	if(m_music) Mix_FreeMusic(m_music);
}

void GameEngine::iterateEngine()
{
	m_gameStates[m_currentState]->updateLoop();

	m_homeTeam->update();
	m_awayTeam->update();
	updateKeyboard();
	m_ball->applyKick();
	m_ball->move();
	if((m_homeTeam->controlState() == Player::KickOnly) && (m_ball->velocity().length() > 0.01)) {
		setSubMode(InPlay, false);
	}

	if(m_ballPrepared) {
		if(m_ball->velocity().length() < 0.01) {
			m_ball->setPosition(m_ballPreparedPosition);
			m_ball->setVelocity(Point3D());
			m_ballPrepared = false;
		}
	}

	m_camera.update();

	if(isGameInProgress()) {
		switch(m_subMode) {
			case InPlay :
				if(m_pitch->crossesSideLine(Segment(m_ball->lastPosition(), m_ball->position()))) {
					setBallPreparedPosition(m_pitch->lastIntersection());
					setSubMode(ThrowIn, (GameEngine::lastTeamTouch() == m_awayTeam), false, m_pitch->lastIntersection());
				} else if(m_pitch->crossesGoalLine(Segment(m_ball->lastPosition(), m_ball->position()))) {
					if(m_pitch->entersGoal(Segment(m_ball->lastPosition(), m_ball->position()))) {
						if(m_pitch->inTopHalf(m_pitch->lastIntersection()) == m_homeTeam->topHalf()) {
							++m_awayScore;
							playSound(CrowdBoo);
						} else {
							playSound(CrowdCheer);
							++m_homeScore;
						}
						setSubMode(GoalCelebration, m_pitch->inTopHalf(m_pitch->lastIntersection()) == m_awayTeam->topHalf());
					} else if (m_pitch->inTopHalf(m_pitch->lastIntersection())==GameEngine::lastTeamTouch()->topHalf()) {
						setSubMode(Corner, (GameEngine::lastTeamTouch() == m_awayTeam), m_pitch->inLeftHalf(m_pitch->lastIntersection()));
					} else {
						playSound(CrowdOoh);
						setBallPreparedPosition(m_pitch->closestGoalKick(m_pitch->lastIntersection()));
						setSubMode(GoalKick, (GameEngine::lastTeamTouch() == m_awayTeam), m_pitch->inLeftHalf(m_pitch->lastIntersection()));
					}
				}
				break;
			case Corner :
				if(m_homeTeam->ready() && m_awayTeam->ready() && (!m_ballPrepared)) {
					if((m_homeTeam->controlState() != Player::KickOnly) || (m_awayTeam->controlState() != Player::KickOnly)) {
						m_homeTeam->setControlState(Player::KickOnly);
						m_awayTeam->setControlState(Player::KickOnly);
						playSound(RefWhistleShort);
					}
				}
				break;
			case GoalKick :
				if(m_homeTeam->ready() && m_awayTeam->ready() && (!m_ballPrepared)) {
					if((m_homeTeam->controlState() != Player::KickOnly) || (m_awayTeam->controlState() != Player::KickOnly)) {
						m_homeTeam->setControlState(Player::KickOnly);
						m_awayTeam->setControlState(Player::KickOnly);
						playSound(RefWhistleShort);
					}
				}
				break;
			case KickOff :
				if(m_homeTeam->ready() && m_awayTeam->ready() && (!m_ballPrepared)) {
					if((m_homeTeam->controlState() != Player::KickOnly) || (m_awayTeam->controlState() != Player::KickOnly)) {
						m_homeTeam->setControlState(Player::KickOnly);
						m_awayTeam->setControlState(Player::KickOnly);
						playSound(RefWhistleShort);
					}
				}
				break;
			case Penalty :
				setSubMode(InPlay, false);
				break;
			case FreeKick :
				setSubMode(InPlay, false);
				break;
			case ThrowIn :
				if(m_homeTeam->ready() && m_awayTeam->ready() && (!m_ballPrepared)) {
					if((m_homeTeam->controlState() != Player::KickOnly) || (m_awayTeam->controlState() != Player::KickOnly)) {
						m_homeTeam->setControlState(Player::KickOnly);
						m_awayTeam->setControlState(Player::KickOnly);
						playSound(RefWhistleShort);
					}
				}
				break;
			case GoalCelebration :
				setSubMode(KickOff, !m_subModeHome);
				break;
		}
	}
}

void GameEngine::drawFrame()
{
	int left = (int)m_camera.position().x() - m_resX/2;
	int top = (int)m_camera.position().y() - m_resY/2;

	// First of all draw the pitch
	m_pitch->draw(left, top);

	// Now render the players
	m_renderer->update();
	m_renderer->draw(left, top);

	// Finally render additional information depending on the gamestate
	m_gameStates[m_currentState]->renderFrame();

	// Optionally draw frames per second
	if(m_displayFPS) {
		std::ostringstream str4;
		str4 << "FPS : " << m_framesPerSecond;
		m_scoreFont->write(m_screen, str4.str().c_str(), m_screen->w - 10 - m_scoreFont->getTextWidth(str4.str().c_str()), 40);
	}

	// Now put everything on screen
	SDL_Flip(m_screen);
}

bool GameEngine::finished()
{
	return m_finished;
}


void GameEngine::setTimer(TimerState state)
{
	m_timerState = state;

	if(state==Restart) {
		m_timer = 0;
		m_timerState = Start;
	}
}

GameEngine::TimerState GameEngine::timerState() const
{
	return m_timerState;
}

uint GameEngine::timer() const
{
	return m_timer;
}

void GameEngine::setState(GameState state)
{
	// First leave the current gamestate (if any)
	if((m_currentState >= 0) && (m_currentState < (int) m_gameStates.size()))
	{
		DEBUG("Leaving gamestate " << m_currentState);
		m_gameStates[m_currentState]->leaveState();
	}
	// Now remember the new gamestate and enter it (if any)
	m_currentState=state;
	if((m_currentState >= 0) && (m_currentState < (int) m_gameStates.size()))
	{
		DEBUG("Entering gamestate " << m_currentState);
		m_gameStates[m_currentState]->enterState();
	}

}

void GameEngine::updateKeyboard()
{
	SDL_Event event;
	static bool quitting = false;

	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) m_finished = 1;
	}

	Uint8 *keys = SDL_GetKeyState(NULL);

	m_homeController->updateController(keys);
	m_awayController->updateController(keys);

	m_gameStates[m_currentState]->update(keys);

	if(keys[m_quitKey]) {
		if (m_currentState >= 0 && (m_currentState < (int) m_gameStates.size()) && m_currentState == TitleScreen) {
			m_finished = 1;
		} else {
			quitting = true;
		}
	} else {
		if(quitting) {
			setState(TitleScreen);
			quitting = false;
		}
	}
}

void GameEngine::setSubMode(SubMode mode, bool home, bool left, const Point3D &pos)
{
	m_subMode = mode;

	switch(mode) {
		case InPlay :
			DEBUG("Prepare InPlay");
			m_homeTeam->setControlState(Player::Full);
			m_awayTeam->setControlState(Player::Full);
			setTimer(Start);
			break;
		case KickOff :
			DEBUG("Prepare KickOff");
			m_homeTeam->setupKickoff(home);
			m_awayTeam->setupKickoff(!home);
			setTimer(Stop);
			setBallPreparedPosition(m_pitch->centerSpot());
			break;
		case Corner :
			DEBUG("Prepare Corner");
			m_homeTeam->setupCorner(left, home);
			m_awayTeam->setupCorner(left, !home);
			setTimer(Stop);
			setBallPreparedPosition(m_pitch->cornerSpot(left, home ? m_awayTeam->topHalf() : m_homeTeam->topHalf()));
			playSound(RefWhistleTwice);
			break;
		case ThrowIn :
			DEBUG("Prepare ThrowIn");
			m_homeTeam->setupThrowIn(pos, home);
			m_awayTeam->setupThrowIn(pos, !home);
			setTimer(Stop);
			setBallPreparedPosition(pos);
			playSound(RefWhistleTwice);
			break;
		case GoalKick :
			DEBUG("Prepare GoalKick");
			WARN("FIXME : Goal Kicks are always on the floor at present.");
			m_homeTeam->setupGoalKick(home, left, true);
			m_awayTeam->setupGoalKick(!home, left, true);
			setTimer(Stop);
			setBallPreparedPosition(m_pitch->goalKickSpot(left, home ? m_homeTeam->topHalf() : m_awayTeam->topHalf()));
			playSound(RefWhistleTwice);
			break;
		case GoalCelebration :
			DEBUG("Prepare GoalCelebration");
			WARN("No goal celebration at present, go straight to kick off.");
			playSound(RefWhistleTwice);
			break;
		case Penalty:
		case FreeKick:
		default :
			WARN("Warning - setSubMode called for mode "  << mode << " is yet unwritten ");
			break;
	}

	m_subModeHome = home;
}

void GameEngine::setBallPreparedPosition(Point3D pos)
{
	m_ballPrepared = true;
	m_ballPreparedPosition = pos;
}

void GameEngine::setMusic(std::string musicFile)
{
	if(m_music) {
		Mix_FreeMusic(m_music);
		m_music = 0;
	}
	if(m_audioOpen) {
		for (int i = 0; i < data_dirs; i++) {
			std::string filename = std::string(data_dir[i]);
			filename.append("/");
			filename.append(musicFile.c_str());
			m_music = Mix_LoadMUS(filename.c_str());
			if (m_music) break;
		}

		if(m_music == 0) {
			WARN("could not load Title music : " << Mix_GetError());
		} else {
			Mix_PlayMusic(m_music, -1);
		}
	}
}

void GameEngine::setCameraFollows(CameraFollows follows)
{
	switch(follows)
	{
	case CameraPattern :
		m_camera.setTarget(0);
		break;
	case CameraBall :
		m_camera.setTarget(m_ball);
		break;
	default :
		m_camera.setTarget(0);
		break;
	}
}

void GameEngine::initialiseMatch()
{
	m_ball->setPosition(m_pitch->centerSpot());
	m_ball->setVelocity(Point3D());
	m_camera.setDestination(Point3D(512, 1024));
	m_ballPrepared = false;
	setSubMode(InPlay, false);
	setTimer(Restart);
	setTimer(Stop);
	m_homeScore = 0;
	m_awayScore = 0;
}

Player *GameEngine::addPlayer(std::string skin, std::string playerMarker, Team *team, bool goalkeeper)
{
	Player *p = new Player(m_renderer, skin, playerMarker, m_pitch, team, m_ball, goalkeeper);

	m_playerList.push_back(PlayerContainer(p));

	return p;
}

void GameEngine::setHalfLength(uint halfLength)
{
	m_halfLength = halfLength;
}

uint GameEngine::halfLength() const
{
	return m_halfLength;
}

void GameEngine::setupHalfTime()
{
	m_homeTeam->setupHalfTime();
	m_awayTeam->setupHalfTime();
}

bool GameEngine::finished() const
{
	return m_finished;
}

void GameEngine::setFinished(bool finished)
{
	m_finished = finished;
}

Controller *GameEngine::controller(ControllerType controller)
{
	switch(controller) {
	case HomeController :
		return m_homeController;
	case AwayController :
		return m_awayController;
	default:
		return 0;
	}
}

SDL_Surface *GameEngine::screen()
{
	return m_screen;
}

Team *GameEngine::team(TeamTypes team)
{
	switch(team) {
	case HomeTeam :
		return m_homeTeam;
	case AwayTeam :
		return m_awayTeam;
	default:
		return 0;
	}
}

Mix_Chunk *GameEngine::gameSound(GameSounds sound)
{
	Mix_Chunk *result = 0;
	if((sound>=0) && (sound<= (int) m_sounds.size())) {
		result = m_sounds[sound];
	}
	return result;
}

void GameEngine::playSound(GameSounds sound)
{
	if((sound>=0) && (sound<= (int) m_sounds.size())) {
		if(m_sounds[sound]) {
			Mix_PlayChannel(-1, m_sounds[sound], 0);
		}
	}
}

uint GameEngine::score(TeamTypes team) const
{
	switch(team)
	{
	case HomeTeam :
		return m_homeScore;
	case AwayTeam :
		return m_awayScore;
	default:
		return 0;
	}
}

void GameEngine::setFullScreen(bool fullscreen)
{
	bool old = m_fullscreen;
	m_fullscreen = fullscreen;
	if (old != m_fullscreen)
	SDL_WM_ToggleFullScreen(m_screen);
}

bool GameEngine::fullScreen() const
{
	return m_fullscreen;
}

bool GameEngine::isGameInProgress()
{
	if(m_currentState >= 0) {
		return m_gameStates[m_currentState]->isGameInProgress();
	}
	return false;
}
