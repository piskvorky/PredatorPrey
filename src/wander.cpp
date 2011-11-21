////////////////////////////////////////////////////////////////////////////////
// file: wander.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// wander goal
// brownian motion
//////////////////////////////////////////////////////////////////////////////////

#include "agent.h"
#include "wander.h"
#include "virtualtime.h"
#include "demo.h" // frand(), M_PI

const TVirtTime MIN_WANDER_TICK = (TVirtTime)0.1;

Wander::Wander() {
    m_name      = "Wander"; // must match class name
    m_owner     = NULL;
}

void Wander::init   ( IEOwner * owner ) {
    m_owner = dynamic_cast<CAgent *> (owner);
}

bool Wander::start () {
    m_state = Go;
	m_owner->setMaxSpeed(m_owner->getWalkSpeed());
	m_dirTick = g_game->m_time->tickNow();
    newDirection();
    return false;
}

bool Wander::update () {
    switch ( m_state ) {
    case Go:
		m_dirTick = g_game->m_time->tickNow();
        if ( m_dirTick > m_tickChangeDir )
            newDirection();
        break;
    }
    return true;
}

bool Wander::finish () {
    return true;
}

IEOwner * Wander::getOwner () {
    return m_owner;
}

const char * Wander::getName() {
    return m_name.c_str();
}

void Wander::newDirection () {
    float r = (randf() * 2 * M_PIf);
	Vector2d steer(cosf(r), sinf(r));
	Vector2d borders = m_owner->getBordersSteer(1);
	borders.scale(1.0f);
	steer = steer + borders;
	m_owner->setSteer(steer);

	m_tickChangeDir = g_game->m_time->tickNow() + MIN_WANDER_TICK;// + ( r * 10.0f );
}