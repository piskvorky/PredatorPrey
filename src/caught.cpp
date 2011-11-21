////////////////////////////////////////////////////////////////////////////////
// file: caught.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// enemy caught event
//////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "agent.h"
#include "caught.h"

const float CAUGHT_DISTANCE = 0.008f;

Caught::Caught() {
	m_name      = "Caught";
	m_owner     = NULL;
}

void Caught::init(IEOwner * owner) {
	m_owner = dynamic_cast<CAgent *> (owner);
}

bool Caught::update () {
	CAgent * nmy = m_owner->getEnemy();
	assert(nmy != NULL);
	return m_owner->distanceTo(nmy) <= CAUGHT_DISTANCE;
}

IEOwner * Caught::getOwner () {
	return m_owner;
}

const char * Caught::getName() {
	return m_name.c_str();
}

