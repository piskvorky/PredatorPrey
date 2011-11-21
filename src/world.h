#ifndef _world_h_
#define _world_h_

#include <assert.h>
#include <list>
#include "demo.h"

class CGridCell {
private:
    float m_drop;
    float m_height[4];
    float m_grass;
public:
    CGridCell(float grass, const float height[4]);
    void addGrass(float diff) { m_grass = aminmax(0.0f, m_grass + diff, 1.0f); }
    void drawCell(float cellSize) const; // defined in gfx.cpp

    inline float getGrass() const { return m_grass; }
    inline float getDrop() const { return m_drop; }
};

class CPeak {
public:
    Vector2d m_position;
    float m_radius;
    CPeak(const Vector2d &pos, float radius) : m_position(pos), m_radius(radius) {};
    inline float heightAt(const Vector2d &pos) const {
        float dist = (m_position - pos).length();
        float height = 0.001f / (0.01f + dist * dist); // FIXME better func?
        if (height > 1.0f)
            height = 1.0f;
        return height;
    }
};

class CTerrain {
    std::list <CPeak *> m_peaks;
public:
    void addPeak(CPeak *peak) {	m_peaks.push_back(peak); }
    float heightAt(const Vector2d &pos) const {
        float height = 0.0f;
        std::list <CPeak *>::const_iterator peak;
        for (peak = m_peaks.begin(); peak != m_peaks.end(); peak++)
            height += (*peak)->heightAt(pos);
        if (height > 2.0f)
            height = 2.0f;
        return height;
    }
    ~CTerrain() {
        std::list <CPeak *>::iterator peak;
        for (peak = m_peaks.begin(); peak != m_peaks.end(); peak++)
            delete (*peak);
        m_peaks.clear();
    }
};

class CWorld {
private:
    CGridCell **m_grid;
    CTerrain terrain;
public:
    int m_gridSize;

    void initTerrain(int numPeaks);
    CWorld(int gridSize, int terrainPeaks);
    ~CWorld();
    void drawWorld() const; // defined in gfx.cpp

    inline int indexAt(int x, int y) const {
        return x + y * m_gridSize;
    }
    inline int isTile(const Vector2d &pos) const {
        assert(pos.m_x >= 0 && pos.m_x <= 1 && pos.m_y >= 0 && pos.m_y <= 1);
        int posX = (int)(pos.m_x * m_gridSize);
        int posY = (int)(pos.m_y * m_gridSize);
        return indexAt(posX, posY);
    }
    inline Vector2d isWorldPos(int index) const {
        int posX = index % m_gridSize;
        int posY = index / m_gridSize;
        float halfOffset = (1.0f / m_gridSize) / 2;
        return Vector2d(halfOffset + 1.0f * posX / m_gridSize, halfOffset + 1.0f * posY / m_gridSize);
    }
    inline CGridCell * cellAt(int x, int y) const {
        assert(x >= 0 && x < m_gridSize && y >= 0 && y < m_gridSize);
        int index = indexAt(x, y);
        return m_grid[index];
    }
    inline CGridCell * cellAt(int index) const {
        assert(index >= 0 && index < m_gridSize * m_gridSize);
        return m_grid[index];
    }
    inline float getHeight(const Vector2d &pos) const {
        return terrain.heightAt(pos);
    }
};

#endif
