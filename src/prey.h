#ifndef _prey_h_
#define _prey_h_

#include "agent.h"

class CPrey : public CAgent {
protected:
	float m_grazeSpeed;
public:
	CPrey();
	int processAgentConstant();
	int processAgentPeriodic();
	void makeBrain();
	void graze();

	void drawAgent() const; // defined in gfx.cpp
	float getStaminaBurnSpeed() const;
	Vector2d getFoodSteer(float) const;
};

#endif
