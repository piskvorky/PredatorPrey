////////////////////////////////////////////////////////////////////////////////
// file: exhausted.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// agent out of stamina event
//////////////////////////////////////////////////////////////////////////////////

#include "agent.h"
#include "exhausted.h"

const float EXHAUSTION_THRESHOLD = 0.1f;

Exhausted::Exhausted() {
	m_name      = "Exhausted";
	m_owner     = NULL;
}

void Exhausted::init( IEOwner * owner ) {
	m_owner = dynamic_cast<CAgent *> (owner);
}


bool Exhausted::update () {
	return m_owner->getStamina() < EXHAUSTION_THRESHOLD;
}


IEOwner * Exhausted::getOwner () {
	return m_owner;
}

const char * Exhausted::getName() {
	return m_name.c_str();
}

