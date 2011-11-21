////////////////////////////////////////////////////////////////////////////////
// file: prey.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// prey is one type of agents, capable of eating grass
// its high-level behaviour is specified in brains.cpp
//////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "prey.h"
#include "agent.h"
#include "game.h"

///////////////////////////////////////////////////////////////////
// CPrey::CPrey
///////////////////////////////////////////////////////////////////
CPrey::CPrey() {
	initialize();
	m_sight = 0.2f;
	m_walkSpeed = 0.15f;
	m_runSpeed = 0.7f;
	m_grazeSpeed = 0.6f;
	m_type = PREY;
}

///////////////////////////////////////////////////////////////////
// CPrey::graze
// eat grass at current position
///////////////////////////////////////////////////////////////////
void CPrey::graze() {
	CWorld *world = g_game->m_world;
	int index = world->isTile(this->getPosition());
	int cellX = index % world->m_gridSize;
	int cellY = index / world->m_gridSize;
	assert(cellX >=0 && cellX < world->m_gridSize && cellY >=0 && cellY < world->m_gridSize);
	world->cellAt(cellX, cellY)->addGrass(-(float)g_game->m_time->lastFrameTicks() * this->m_grazeSpeed);
}

///////////////////////////////////////////////////////////////////
// CPrey::processAgentConstant
// things that must be done every frame
///////////////////////////////////////////////////////////////////
int CPrey::processAgentConstant() {
	move();
	graze();
	return 0;
}

///////////////////////////////////////////////////////////////////
// CPrey::processAgentPeriodic
// things that may be done at arbitrary frequency ("thinking")
// omitting to call this will result in unresponsive, dull agents but
// should not otherwise interfere with the simulation
///////////////////////////////////////////////////////////////////
int CPrey::processAgentPeriodic() {
	think();
	m_lastUpdate = g_game->m_time->tickNow();
	return 0;
}

///////////////////////////////////////////////////////////////////
// CPrey::getStaminaBurnSpeed
// the speed at which agent burns out stamina (directly proportional to current velocity)
///////////////////////////////////////////////////////////////////
float CPrey::getStaminaBurnSpeed() const {
	if (this->getSpeed() < (this->m_walkSpeed + this->m_runSpeed) / 2)
		return -1.0f;
	else
		return -1.0f;
}

///////////////////////////////////////////////////////////////////
// CPrey::getFoodSteer
// finds the place with the most grass within a given radius
///////////////////////////////////////////////////////////////////
Vector2d CPrey::getFoodSteer(const float radius) const {
	Vector2d result(0.0f, 0.0f);
	CWorld *world = g_game->m_world;
	int index = world->isTile(this->getPosition());
	int cellX = index % world->m_gridSize;
	int cellY = index / world->m_gridSize;
	int rad = (int)(radius * world->m_gridSize) + 1;
	float bestValue = -1.0f;
	int bestIndex = -1;
	double halfOffset = (1.0f / world->m_gridSize) / 2;
	double over = 1.0 / world->m_gridSize;
	double diffx = (halfOffset + (cellX - rad) * over) - getPosX();
	double diffy = (halfOffset + (cellY - rad) * over) - getPosY();

	for (int i = cellX - rad; i < cellX + rad; i++) {
		if (i >= 0 && i < world->m_gridSize ) {
			for (int j = cellY - rad; j < cellY + rad; j++) {
				if (j >= 0 && j < world->m_gridSize)  {
					float sqrLength = (float)(diffx * diffx + diffy * diffy);
					if (sqrLength < radius * radius) {
						int index = world->indexAt(i, j);
						float val = world->cellAt(index)->getGrass() / (0.1f + sqrtf(sqrLength));
						if (val > bestValue) {
							bestValue = val;
							bestIndex = index;
						}
					}
				}
				diffy += over;
			}
		}
		diffx += over;
		diffy -= 2 * rad * over;
	}
	if (bestIndex >= 0) {
		result = (world->isWorldPos(bestIndex) - getPosition());
		result.scale(bestValue);
	}
	return result;
}
