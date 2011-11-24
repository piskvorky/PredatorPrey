////////////////////////////////////////////////////////////////////////////////
// file: predator.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// predator is one type of agents. its high-level behaviour is specified in brains.cpp
// unlike prey, it does not bring any new abilities to CAgent and only implements its abstract methods
//////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "predator.h"
#include "agent.h"
#include "game.h"

///////////////////////////////////////////////////////////////////
// CPredator::CPredator
///////////////////////////////////////////////////////////////////
CPredator::CPredator() {
    initialize();
    m_sight = 0.25f;
    m_walkSpeed = 0.08f;
    m_runSpeed = 0.9f;
    m_type = PREDATOR;
}

///////////////////////////////////////////////////////////////////
// CPredator::processAgentConstant
// things that must be done every frame
///////////////////////////////////////////////////////////////////
int CPredator::processAgentConstant() {
    move();
    return 0;
}

///////////////////////////////////////////////////////////////////
// CPredator::processAgentPeriodic
// things that may be done at arbitrary frequency ("thinking")
// omitting to call this will result in unresponsive, dull agents but
// should not otherwise interfere with the simulation
///////////////////////////////////////////////////////////////////
int CPredator::processAgentPeriodic() {
    think();
    m_lastUpdate = g_game->m_time->tickNow();
    return 0;
}

///////////////////////////////////////////////////////////////////
// CPredator::getStaminaBurnSpeed
// the speed at which agent burns out stamina (directly proportional to current velocity)
///////////////////////////////////////////////////////////////////
float CPredator::getStaminaBurnSpeed() const {
    if (this->getSpeed() < (this->m_walkSpeed + this->m_runSpeed) / 2)
        return -1.0f;
    else
        return +1.3f;
}
