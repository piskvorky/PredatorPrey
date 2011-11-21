////////////////////////////////////////////////////////////////////////////////
// file: virtual time.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// real time / simulation time management
//////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4800)

#define WIN32_LEAN_AND_MEAN

#include <cassert>
#include <cstdlib>

#include "virtualtime.h"

#ifdef WIN32
#include <windows.h> // QueryPerformance
#include <mmsystem.h> // timeGetTime, timeBeginPeriod, timeEndPeriod
#else
#include <sys/time.h>
#endif


///////////////////////////////////////////////////////////////////
// CVirtualTime::CVirtualTime
///////////////////////////////////////////////////////////////////
CVirtualTime::CVirtualTime() {
#ifdef WIN32
    LARGE_INTEGER foo;
    m_hwCounterPresent = QueryPerformanceFrequency(&foo);
    if (!m_hwCounterPresent) // hardware counter not available
        timeBeginPeriod(1);
#endif
    reset();
    setSpeed(0.0f); // simulation is initially paused
}

///////////////////////////////////////////////////////////////////
// CVirtualTime::~CVirtualTime
///////////////////////////////////////////////////////////////////
CVirtualTime::~CVirtualTime() {
#ifdef WIN32
    if (!m_hwCounterPresent)
        timeEndPeriod(1);
#endif
}

///////////////////////////////////////////////////////////////////
// CVirtualTime::timeNow
// get current time in seconds
///////////////////////////////////////////////////////////////////
TRealTime CVirtualTime::timeNow() const {
    TRealTime result;
#ifdef WIN32
    if (m_hwCounterPresent) {
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        result = (TRealTime)(1.0 * t.QuadPart / f.QuadPart);
    } else {
        result = (TRealTime)(1.0 * timeGetTime() / 1000);
    }
#else
    timeval now;
    gettimeofday(&now, NULL);
    result = now.tv_sec + now.tv_usec / 1000000.0;
#endif
    return result;
}

///////////////////////////////////////////////////////////////////
// CVirtualTime::reset
// reset timer back to 0
///////////////////////////////////////////////////////////////////
void CVirtualTime::reset() {
    m_tick = (TVirtTime)0.0;
    m_frameStartedTick = (TVirtTime)0.0;
    m_currentTime = timeNow();
    m_lastTime = m_currentTime;
    m_lastFrameDiff = (TVirtTime)0.0;
}

///////////////////////////////////////////////////////////////////
// CVirtualTime::setSpeed
// set simulation speed
///////////////////////////////////////////////////////////////////
void CVirtualTime::setSpeed(float _speed) {
    assert(_speed >= 0);
    newFrame(); // speed cannot vary within one frame
    m_speed = _speed;
}

///////////////////////////////////////////////////////////////////
// CVirtualTime::newFrame
// calculate time spent in last frame, mark new frame start
///////////////////////////////////////////////////////////////////
void CVirtualTime::newFrame() {
    TRealTime now = timeNow();
    m_lastFrameDiff = m_speed * (TVirtTime)(now - m_currentTime); // mark simulation time spent during previous frame
    m_tick += m_lastFrameDiff; // keep track of overall simulation time
    m_lastTime = m_currentTime;
    m_currentTime = now;
    m_frameStartedTick = tickNow(); // mark new frame start
}
