
#include <assert.h>

#include "agent.h"
#include "flee.h"
#include "virtualtime.h"
#include "demo.h"

Flee::Flee() {
	m_name      = "Flee"; // must match class name
	m_owner     = NULL;
}

void Flee::init ( IEOwner * owner )
{
	m_owner = dynamic_cast<CAgent *> (owner);
}

bool Flee::start () {
	m_state = Go;
	m_owner->setMaxSpeed(m_owner->getRunSpeed());
	m_minDirTick = 0.05f;
	newDirection(m_minDirTick);
	m_owner->m_done = false;
	return false;
}

void Flee::newDirection(TVirtTime minTick) {
	CAgent *nmy = m_owner->getEnemy();
	assert(nmy != NULL);
	Vector2d n = nmy->getPosition() - m_owner->getPosition();
	float newdir;
	if (rand(2) == 0 && m_owner->distanceTo(nmy) < 0.05f) // try dodging
		newdir = (float)(atan2f(n.m_y, -n.m_x) + (2 * rand(2) - 1) * 0.55f * M_PIf);
	else
		newdir = (float)(atan2f(n.m_y, -n.m_x) + randf() * 0.25f * M_PIf);
	Vector2d escape(cosf(newdir), -sinf(newdir)); // run away!
	escape.normalize();
	Vector2d separation = m_owner->getCohesionSteer(0.2f, 0.3f * M_PIf); // avoid other agents, too
	separation.normalize();
	Vector2d borders = m_owner->getBordersSteer(4); // avoid getting near world borders

	escape.scale(4.0);
	separation.scale(-0.0f);
	borders.scale(10.0f);

	Vector2d dir = separation + escape + borders;
	dir.stretchTo(1000.0);
	m_owner->setSteer(dir);

	m_dirTick = g_game->m_time->tickNow() + minTick;
}

bool Flee::update () {
	CAgent *nmy = m_owner->getEnemy();
	assert(nmy != NULL);
	switch ( m_state ) {
	case Go:
		if (g_game->m_time->tickNow() > m_dirTick) {
			if (nmy->m_active && m_owner->canSee(nmy))
				newDirection(m_minDirTick);
			else {
				newDirection(5 * m_minDirTick); // keep running for a while, just in case
				m_owner->m_done = true;
			}
		}
		break;
	}
	return true;
}

bool Flee::finish () {
	return g_game->m_time->tickNow() > m_dirTick;
}

IEOwner * Flee::getOwner () {
	return m_owner;
}

const char * Flee::getName() {
	return m_name.c_str();
}

