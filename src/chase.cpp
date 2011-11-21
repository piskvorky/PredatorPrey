////////////////////////////////////////////////////////////////////////////////
// file: chase.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// chase event - set steer in the direction of enemy, with possible anticipation
//////////////////////////////////////////////////////////////////////////////////

#include "agent.h"
#include "chase.h"
#include "assert.h"

Chase::Chase() {
	m_name      = "Chase";
	m_owner     = NULL;
}

void Chase::init   ( IEOwner * owner ) {
	m_owner = dynamic_cast<CAgent *> (owner);
}

bool Chase::start () {
	m_state = Go;
	m_owner->setMaxSpeed(m_owner->getRunSpeed());
	return false;
}

bool Chase::update () {
	switch ( m_state ) {
	case Go:
		CAgent *nmy = m_owner->getEnemy();

		assert(nmy != NULL);
		nmy->m_marked = true;
		if (!nmy->m_active || !m_owner->canSee(nmy))  // target got away
			return false;
		Vector2d desiredVel = nmy->getPosition() - m_owner->getPosition();
		//intercept
		/*
		if (m_owner->getSpeed() > 0.001f) {
			float timeTaken = desiredVel.length() / m_owner->getSpeed();
			desiredVel = desiredVel + nmy->getVelocity() * timeTaken;
		}
		*/
		Vector2d steer = desiredVel;
		steer.stretchTo(10000.0f);
		m_owner->setSteer(steer);
		break;
	}
	return true;
}

bool Chase::finish () {
	return true;
}

IEOwner * Chase::getOwner () {
	return m_owner;
}

const char * Chase::getName() {
	return m_name.c_str();
}

