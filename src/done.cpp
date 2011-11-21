////////////////////////////////////////////////////////////////////////////////
// file: done.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// generic done event
// currently a bit of a hack, state is communicated via owner's m_done property flag
//////////////////////////////////////////////////////////////////////////////////

#include "agent.h"
#include "done.h"

Done::Done() {
	m_name      = "Done";
	m_owner     = NULL;
}

void Done::init( IEOwner * owner ) {
	m_owner = dynamic_cast<CAgent *> (owner);
}

bool Done::update () {
	if (m_owner->m_done) {
		m_owner->m_done = false;
		return true;
	}
	return false;
}

IEOwner * Done::getOwner () {
	return m_owner;
}

const char * Done::getName() {
	return m_name.c_str();
}

