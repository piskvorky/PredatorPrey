////////////////////////////////////////////////////////////////////////////////
// file: world.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// world as parametric height field, approximated by grid with adjustable tile size
// world rendering is based on this tile representation
//////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "world.h"
#include "virtualtime.h"
#include "demo.h"

CGridCell::CGridCell(float grass, const float height[4]) { 
	m_grass = grass;
	float avgHeight = 0.0f;
	for (int i = 0; i < 4; i++) {
		m_height[i] = height[i];
		avgHeight += height[i];
	}
	avgHeight /= 4;
	float maxDiff = 0.0f;
	for (int k = 0; k < 4; k++) {
		float diff = fabsf(avgHeight - m_height[k]);
		if (diff > maxDiff)
			maxDiff = diff;
	}
	m_drop = maxDiff;
}

void CWorld::initTerrain(int numPeaks) {
	for (int i = 0; i < numPeaks; i++) {
		CPeak *peak = new CPeak(Vector2d(randf(), randf()), 0.1f + randf() / 2);
		terrain.addPeak(peak);			
	}
}

CWorld::CWorld(int gridSize, int terrainPeaks) {
	initTerrain(terrainPeaks);
	m_gridSize = gridSize;
	m_grid = (CGridCell **)malloc(sizeof(CGridCell *) * m_gridSize * m_gridSize);
	for (int i = 0; i < m_gridSize; i++)
		for (int j = 0; j < m_gridSize ; j++) {
			int index = indexAt(j, i);
			Vector2d middle = isWorldPos(index);
			float halfCell = (1.0f / m_gridSize) / 2;
			float heights[4];
			heights[0] = terrain.heightAt(middle + Vector2d(-halfCell, -halfCell));
			heights[1] = terrain.heightAt(middle + Vector2d(halfCell, -halfCell));
			heights[2] = terrain.heightAt(middle + Vector2d(halfCell, halfCell));
			heights[3] = terrain.heightAt(middle + Vector2d(-halfCell, halfCell));
			m_grid[index] = new CGridCell(0.0f, heights);
			m_grid[index]->addGrass(1.0f - (2.0f * m_gridSize * m_grid[index]->getDrop()));
		}
}

CWorld::~CWorld() {
	for (int i = 0; i < m_gridSize * m_gridSize; i++)
		delete m_grid[i];
	free(m_grid);
}
