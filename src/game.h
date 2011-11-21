#ifndef _game_h_
#define _game_h_

#include <list>
#include <assert.h>
#include "demo.h"
#include "virtualtime.h"

class CWorld;
class CVirtualTime;
class CAgent;
class CGame;

extern CGame *g_game;

class CCamera {
private:
	float m_fov;
	float m_aspect;
	Vector3d m_position;
	Vector3d m_target;
	Vector3d m_upVector;
public:
	CCamera() : m_fov(100.0f), m_aspect(1.0f), m_position(Vector3d(0.0f, 1.0f, 0.0f)), m_target(Vector3d(0.0f, 0.0f, 0.0f)),
		m_upVector(Vector3d(0.0f, 1.0f, 0.0f)) {};
	Vector3d getPosition() const { return m_position; }
	Vector3d getTarget() const { return m_target; }
	Vector3d getUpVector() const { return m_upVector; }
	float getFOV() const { return m_fov; }
	float getAspectRatio() const { return m_aspect; }
	void setPosition(const Vector3d &pos) { m_position = pos; }
	void setTarget(const Vector3d &target) { m_target = target; }
	void setUpVector(const Vector3d &up) { m_upVector = up; }
	void setFOV(float f) { m_fov = f; }
	void setAspectRatio(float f) { m_aspect = f; }
};

class CTimer {
private:
	EPeriod m_currentPeriod;
	TRealTime m_currentStart;
	TRealTime m_timers[PERIOD_NUM];
	TRealTime m_lastTimers[PERIOD_NUM];
	CVirtualTime * m_virtualTime;
public:
	void initialize(CVirtualTime * vtimer) {
		m_virtualTime = vtimer;
		m_currentPeriod = PERIOD_IDLE;
		resetPeriods();
		for (int i = 0; i < PERIOD_NUM; i++)
			m_lastTimers[i] = 0.0;
	}
	void startPeriod(EPeriod newPeriod) {
		TRealTime timeNow = m_virtualTime->timeNow();
		TRealTime elapsed = timeNow - m_currentStart;
		m_timers[m_currentPeriod] += elapsed;
		m_currentStart = timeNow;
		m_currentPeriod = newPeriod;
	}
	TRealTime getTimeSpent(EPeriod period) const {
		assert(period >= 0 && period < PERIOD_NUM);
		return m_lastTimers[period];
	}
	void resetPeriod(EPeriod period) {
		m_lastTimers[period] = m_timers[period];
		m_timers[period] = 0.0;
	}
	void resetPeriods() {
		startPeriod(PERIOD_IDLE);
		for (int i = 0; i < PERIOD_NUM; i++)
			resetPeriod((EPeriod)i);
	}
};

class CGame {
protected:
	TRealTime m_lastSec;
	unsigned int m_lastFramesRendered;
	unsigned int m_lastFramesBuilt;
	unsigned int m_framesBuilt;
	unsigned int m_framesRendered;
public:
	std::list <CAgent *> m_agents;
	float m_framesBuiltPS;
	float m_framesRenderedPS;
	CTimer m_periodTimer;
	CVirtualTime *m_time;
	CWorld *m_world;
	CCamera *m_camera;

	CGame(int granularity, int peaks, int preyNum, int predatorNum);
	~CGame();
	int registerAgent(CAgent *);
	int updateAI(TRealTime maxTime);
	int buildFrameConstant();
	int buildFramePeriodic();
	void newFrame() { m_time->newFrame(); m_framesRendered++; }
	void newBuild() { m_framesBuilt++; }
	TRealTime getTimeElapsedSinceSecond(TRealTime now) const { return now - m_lastSec; }
	unsigned int getRenderedSinceSecond() const { return m_framesRendered - m_lastFramesRendered; }
	unsigned int getBuiltSinceSecond() const { return m_framesBuilt - m_lastFramesBuilt; }
	unsigned int getDesiredFramesDone(TRealTime now, unsigned int fps) const { return (unsigned int)(getTimeElapsedSinceSecond(now) * fps); }

	void renderFrame(); // defined in gfx.cpp
	int buildFrame();
	int updateFPS(TRealTime now);
};

#endif
