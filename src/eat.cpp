////////////////////////////////////////////////////////////////////////////////
// file: eat.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// eat goal
// 'eating' is waiting a specified period of simulation time, immobile
//////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "agent.h"
#include "eat.h"
#include "virtualtime.h"

const TVirtTime EAT_TICKS = (TVirtTime)0.6;

Eat::Eat() {
	m_name      = "Eat"; // must match class name
	m_owner     = NULL;
}

void Eat::init (IEOwner * owner) {
	m_owner = dynamic_cast<CAgent *> (owner);
}

bool Eat::start () {
	CAgent *nmy = m_owner->getEnemy();
	assert(nmy != NULL);
	m_state = Go;
	m_tickChangeDir = g_game->m_time->tickNow() + EAT_TICKS;
	m_owner->setMaxSpeed(0.0f);
	nmy->m_active = false;
	m_owner->m_done = false;
	return false;
}

bool Eat::update () {
	switch ( m_state ) {
	case Go:
        if ( g_game->m_time->tickNow() > m_tickChangeDir )
			m_owner->m_done = true;
		break;
	}
	return true;
}

bool Eat::finish () {
	return true;
}

IEOwner * Eat::getOwner () {
	return m_owner;
}

const char * Eat::getName() {
	return m_name.c_str();
}

