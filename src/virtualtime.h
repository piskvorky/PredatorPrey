#ifndef _virtualtime_h_
#define _virtualtime_h_


typedef float TVirtTime;
typedef double TRealTime;

class CVirtualTime {
private:
    bool m_hwCounterPresent;
    float m_speed; // simulation speed; set to 0 to pause the simulation
    TRealTime m_currentTime; // real time
    TRealTime m_lastTime;
    TVirtTime m_frameStartedTick; // marks the simulation time the current frame started
    TVirtTime m_tick; // simulation time
    TVirtTime m_lastFrameDiff; // how much simulation time elapsed in the last frame
public:
    CVirtualTime();
    ~CVirtualTime();
    void reset();
    void setSpeed(float newSpeed);
    void newFrame();

    // real time & simulation time query routines
    TRealTime timeNow() const;
    TRealTime timeElapsedSinceNewFrame() const { return timeNow() - m_currentTime; }
    TVirtTime lastFrameTicks() const { return m_lastFrameDiff; }
    TVirtTime tickNow() const { return m_tick; }
    float getSpeed() const { return m_speed; }
    bool getHWCounterPresent() const { return m_hwCounterPresent; }
};

#endif
