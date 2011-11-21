////////////////////////////////////////////////////////////////////////////////
// file: agent.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// abstract base class for agents
//////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <assert.h>
#include "ie.h"
#include "agent.h"
#include "virtualtime.h"
#include "demo.h"
#include "game.h"
#include "world.h"

///////////////////////////////////////////////////////////////////
// CAgent::move
// adjust agent's position, taking into account the simulation time elapsed since last frame
///////////////////////////////////////////////////////////////////
void CAgent::move() {
	const float MAX_SPEED = 1.0f;
	TVirtTime lastFrameTicks = g_game->m_time->lastFrameTicks();
	Vector2d velDiff = this->getSteer() * lastFrameTicks; // m / (s^2) * s = m / s
	this->m_velocity = this->m_velocity + velDiff; // m / s + m / s

	if (this->getSpeed() > this->getMaxSpeed()) // cannot exceed agent's maximal velocity
		m_velocity.stretchTo(this->getMaxSpeed());
	if (this->getSpeed() > MAX_SPEED) // cannot exceed physical barrier for speed
		m_velocity.stretchTo(MAX_SPEED);

	if (this->getStamina() > 0.0f) {
		Vector2d posDiff = this->m_velocity * lastFrameTicks; // m / s * s = m
		m_position = m_position + posDiff; // m + m
		m_position.clamp(Vector2d(0.0f, 0.0f), Vector2d(0.9999f, 0.9999f)); // cannot go outside world borders
	}

	float stamDiff = lastFrameTicks * (-this->getStaminaBurnSpeed()); // adjust stamina based on current velocity
	m_stamina = aminmax(0.0f, m_stamina + stamDiff, 1.0f);
}

///////////////////////////////////////////////////////////////////
// CAgent::initialize
// set everything to 0, randomize start position, activate
///////////////////////////////////////////////////////////////////
void CAgent::initialize() {
	m_velocity = Vector2d(0.0f, 0.0f);
	setMaxSpeed(0.0f);
	setSteer(Vector2d(0.0f, 0.0f));
	m_stamina = 1.0f;
	setEnemy(NULL);
	m_position = Vector2d(0.3f + randf() / 2.0f, 0.3f + randf() / 2.0f);
	m_active = true;
	makeBrain();
}

///////////////////////////////////////////////////////////////////
// CAgent::getSeparationSteer
// nearby agent avoidance, used in flocking
///////////////////////////////////////////////////////////////////
Vector2d CAgent::getSeparationSteer(float radius, float angle) const {
	Vector2d result(0.0f, 0.0f);
	for (std::list <CAgent *>::iterator _agent = g_game->m_agents.begin(); _agent != g_game->m_agents.end(); _agent++) {
			CAgent *agent = *_agent;
			if (agent->m_active && isNeighbour(agent, radius, angle)) {
				Vector2d dx = this->getPosition() - agent->getPosition();
				float length = dx.length();
				if (length > 0) {
					dx.scale(1.0f / (length * length));
				}
				result = result + dx;
			}
	}
	return result;
}

///////////////////////////////////////////////////////////////////
// CAgent::getCohesionSteer
// nearby agent cohesion. used in flocking
///////////////////////////////////////////////////////////////////
Vector2d CAgent::getCohesionSteer(float radius, float angle) const {
	Vector2d result(0.0f, 0.0f);
	int cnt = 0;
	for (std::list <CAgent *>::iterator _agent = g_game->m_agents.begin(); _agent != g_game->m_agents.end(); _agent++) {
			CAgent *agent = *_agent;
			if (agent->m_active && isNeighbour(agent, radius, angle)) {
				result = result + agent->getPosition();
				cnt++;
			}
	}
	if (cnt > 0) {
		result.scale(1.0f / cnt);
		result = result - getPosition();
	}
	return result;
}

///////////////////////////////////////////////////////////////////
// CAgent::getAlignmentSteer
// nearby agent velocity vector alignment. used in flocking
///////////////////////////////////////////////////////////////////
Vector2d CAgent::getAlignmentSteer(float radius, float angle) const {
	Vector2d result(0.0f, 0.0f);
	int cnt = 0;
	for (std::list <CAgent *>::iterator _agent = g_game->m_agents.begin(); _agent != g_game->m_agents.end(); _agent++) {
			CAgent *agent = *_agent;
			if (agent->m_active && isNeighbour(agent, radius, angle)) {
				Vector2d dx(agent->getVelocity());
				dx.normalize();
				result = result + dx;
				cnt++;
			}
	}
	if (cnt > 0) {
		result.scale(1.0f / cnt);
		result = result - this->getVelocity();
	}
	return result;
}

///////////////////////////////////////////////////////////////////
// CAgent::getBordersSteer
// borders avoidance
///////////////////////////////////////////////////////////////////
Vector2d CAgent::getBordersSteer(int steepness) const {
	Vector2d result(0.0f, 0.0f);
	Vector2d position = getPosition();
	double resx;
	double resy;
	resx = (1 - position.m_x);
	resy = (1 - position.m_y);
	for (int i = 0; i < steepness; i++) {
		resx = resx * resx;
		resy = resy * resy;
	}
	result = result + Vector2d((float)resx, (float)resy);
	resx = (position.m_x);
	resy = (position.m_y);
	for (int i = 0; i < steepness; i++) {
		resx = resx * resx;
		resy = resy * resy;
	}
	result = result + Vector2d((float)-resx, (float)-resy);
	return result;
}

///////////////////////////////////////////////////////////////////
// CAgent::isNeighbour
// agent is considered a neighbour if its within radius and inside fov
// totally naive implementation, major optimizations possible (mipmapping the terrain with respect to agents, caching...)
///////////////////////////////////////////////////////////////////
bool CAgent::isNeighbour(const CAgent *other, float radius, float angle) const {
	if (this == other)
		return false;
	Vector2d dx = other->getPosition() - this->getPosition();
	if (dx.sqrLength() > radius * radius)
		return false; // is too far away?
	Vector2d sx = this->getVelocity();
	float ang0 = atan2f(-sx.m_y, sx.m_x);
	Vector2d nx(cosf(ang0) * dx.m_x - sinf(ang0) * dx.m_y, sinf(ang0) * dx.m_x + cosf(ang0) * dx.m_y);
	float ang1 = atan2f(-nx.m_y, nx.m_x);
	return fabsf(ang1) <= angle; // is inside fov?
}

///////////////////////////////////////////////////////////////////
// CAgent::canSee
///////////////////////////////////////////////////////////////////
bool CAgent::canSee(const CAgent *other) const { // FIXME LOS?
	return sqrDistanceTo(other) < m_sight * m_sight;
}

///////////////////////////////////////////////////////////////////
// CAgent::setSteer
// sets the current steering force for the agent (m / (s^2))
///////////////////////////////////////////////////////////////////
void CAgent::setSteer(const Vector2d & steer) { 
	const float MAX_FORCE = 50.0f;
	m_steer = steer; 
	m_steer.truncate(MAX_FORCE);
}
