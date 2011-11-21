////////////////////////////////////////////////////////////////////////////////
// file: replenish.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// replenish (wait for stamina renewal) event
//////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "agent.h"
#include "replenish.h"

const float STAMINA_OK = 0.95f;

Replenish::Replenish() {
	m_name      = "Replenish";
	m_owner     = NULL;
}

void Replenish::init   ( IEOwner * owner ) {
	m_owner = dynamic_cast<CAgent *> (owner);
}

bool Replenish::start () {
	m_state = Go;
	m_owner->setMaxSpeed(0.0f);
	m_owner->m_done = false;
	return false;
}

bool Replenish::update () {
	switch ( m_state ) {
	case Go:
		if (m_owner->getStamina() >= STAMINA_OK)
			m_owner->m_done = true;
		break;
	}
	return true;
}

bool Replenish::finish () {
	return true;
}

IEOwner * Replenish::getOwner () {
	return m_owner;
}

const char * Replenish::getName() {
	return m_name.c_str();
}

