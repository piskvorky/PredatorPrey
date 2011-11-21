////////////////////////////////////////////////////////////////////////////////
// file: game.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// game is an independent simulation unit, has its own virtual time, world, agents
//////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <cstdlib>

#include "virtualtime.h"
#include "prey.h"
#include "predator.h"
#include "game.h"
#include "world.h"
#include "demo.h"

CGame * g_game;

///////////////////////////////////////////////////////////////////
// CGame::CGame
// initialize everything (timers, speeds) to zero
///////////////////////////////////////////////////////////////////
CGame::CGame(int granularity, int peaks, int preyNum, int predatorNum) {
	m_time = new CVirtualTime();
	m_world = new CWorld(granularity, peaks);
	srand((unsigned int)(m_time->timeNow())); // first world ^ is always identical

	for (int i = 0; i < preyNum; i++)
		registerAgent(new CPrey());
	for (int i = 0; i < predatorNum; i++)
		registerAgent(new CPredator());

	m_camera = new CCamera();
	m_framesBuiltPS = m_framesRenderedPS = 0;
	m_lastFramesRendered = m_framesRendered = 0;
	m_lastFramesBuilt = m_framesBuilt = 0;
	m_periodTimer.initialize(m_time);
	m_lastSec = m_time->timeNow();
}

///////////////////////////////////////////////////////////////////
// CGame::~CGame
// destroys all registered agents as well
///////////////////////////////////////////////////////////////////
CGame::~CGame() {
	delete m_camera;
	for (std::list <CAgent *>::iterator agent = this->m_agents.begin(); agent != this->m_agents.end(); agent++)
		delete (*agent);
	m_agents.clear();
	delete m_world;
	delete m_time;
}

///////////////////////////////////////////////////////////////////
// CGame::registerAgent
// register agent for the simulation
///////////////////////////////////////////////////////////////////
int CGame::registerAgent(CAgent *agent) {
	this->m_agents.push_back(agent);
	return 0;
}

///////////////////////////////////////////////////////////////////
// CGame::updateAI
// register agent for the simulation
///////////////////////////////////////////////////////////////////
int CGame::updateAI(TRealTime maxTime) {
	for (std::list <CAgent *>::iterator agent = this->m_agents.begin(); agent != this->m_agents.end(); agent++) {
		if ((*agent)->m_active)
			(*agent)->processAgentPeriodic();
	}
	return 0;
}

///////////////////////////////////////////////////////////////////
// CGame::buildFramePeriodic
// do some not-so-urgent work
///////////////////////////////////////////////////////////////////
int CGame::buildFramePeriodic() {
	this->updateAI(0.0f);
	return 0;
}

///////////////////////////////////////////////////////////////////
// CGame::buildFrameConstant
// things that must be every frame
///////////////////////////////////////////////////////////////////
int CGame::buildFrameConstant() {
	for (std::list <CAgent *>::iterator agent = m_agents.begin(); agent != m_agents.end(); agent++)
		if ((*agent)->m_active)
			(*agent)->processAgentConstant();
	return 0;
}

///////////////////////////////////////////////////////////////////
// CGame::updateFPS
// update framerate statistics
///////////////////////////////////////////////////////////////////
int CGame::updateFPS(TRealTime now) {
	if (now - m_lastSec > 1.0f) {
		TRealTime elapsed = (TRealTime)1.0;//now - m_lastSec;
		m_framesRenderedPS = (float)(getRenderedSinceSecond() / elapsed);
		m_framesBuiltPS = (float)(getBuiltSinceSecond() / elapsed);
		m_lastFramesBuilt = m_framesBuilt;
		m_lastFramesRendered = m_framesRendered;
		m_lastSec = now;
		m_periodTimer.resetPeriods();
	}
	return 0;
}

