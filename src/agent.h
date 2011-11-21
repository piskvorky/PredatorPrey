#ifndef _agent_h_
#define _agent_h_

#include <math.h>
#include "demo.h"
#include "ieowner.h"
#include "world.h"
#include "game.h"

///////////////////////////////////////////////////////////////////
// CAgent
// abstract base class for agents.  
// extends IEOwner, meaning each agent can have its own brain
///////////////////////////////////////////////////////////////////
class CAgent : public IEOwner {
public:
	enum AgentType { PREDATOR, PREY };

private:
	Vector2d m_position;
	Vector2d m_steer;
	Vector2d m_velocity;
protected:
	TVirtTime m_lastUpdate;
	float m_runSpeed;
	float m_walkSpeed;
	float m_stamina;
	float m_sight;
	float m_maxSpeed;
	CAgent *m_enemy;
	AgentType m_type;
public:
	virtual ~CAgent() {}
	void initialize();

	// non-physical stats (changeable by user)
	bool m_active;
	bool m_done;
	bool m_marked;
	void setSteer(const Vector2d & steer);
	void setEnemy(CAgent * nmy) { m_enemy = nmy; }
	void setMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }

	// engine routines
	virtual int processAgentConstant() = 0;
	virtual int processAgentPeriodic() = 0;
	virtual void drawAgent() const = 0;
	virtual void move();

	// agent status query routines; no changes allowed (consts)
	float getAgentHeight() const { return g_game->m_world->getHeight(m_position); }
	float getMaxSpeed() const { return m_maxSpeed; }
	float getSpeed() const { return m_velocity.length(); }
	float getStamina() const { return m_stamina; }
	float getWalkSpeed() const { return m_walkSpeed; }
	float getRunSpeed() const { return m_runSpeed; }
	Vector2d getSteer() const { return m_steer; }
	Vector2d getPosition() const { return m_position; }
	float getPosX() const { return m_position.m_x; }
	float getPosY() const { return m_position.m_y; }
	Vector2d getVelocity() const { return m_velocity; }
	Vector2d getCohesionSteer(float, float) const;
	Vector2d getSeparationSteer(float, float) const;
	Vector2d getAlignmentSteer(float, float) const;
	Vector2d getBordersSteer(int) const;
	virtual float getStaminaBurnSpeed() const = 0;
	float distanceTo(const CAgent *other) const { return agentDistance(this, other); }
	float sqrDistanceTo(const CAgent *other) const { return sqrAgentDistance(this, other); }
	bool canSee(const CAgent *other) const;
	bool isNeighbour(const CAgent *other, float radius, float angle) const;
	CAgent * getEnemy() const { return m_enemy; }
	AgentType getType() const { return m_type; }

	// static routines
	static float agentDistance(const CAgent *agent1, const CAgent *agent2) { return (agent1->getPosition() - agent2->getPosition()).length(); }
	static float sqrAgentDistance(const CAgent *agent1, const CAgent *agent2) { return (agent1->getPosition() - agent2->getPosition()).sqrLength(); }
};
///////////////////////////////////////////////////////////////////

#endif
