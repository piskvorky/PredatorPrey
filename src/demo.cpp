////////////////////////////////////////////////////////////////////////////////
// file: demo.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// program entry point, initialization and main loop
//////////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "game.h"
#include "agent.h"
#include "virtualtime.h"
#include "world.h"
#include "demo.h"

int s_fps = 40; // desired initial frame rate
int s_bps = 20; // desired initial build rate
float s_speed = 0.2f; // initial simulation speed

const int WORLD_GRANULARITY = 70;
const int WORLD_PEAKS = 20;
const int PREY_NUM = 60;
const int PREDATOR_NUM = 1;

///////////////////////////////////////////////////////////////////
// randf
// return a pseudorandom number in the range <0.0f, 1.0f>
///////////////////////////////////////////////////////////////////
float randf() { return 1.0f * rand() / RAND_MAX; }

///////////////////////////////////////////////////////////////////
// rand
// return a pseudorandom number in the range <0, min(maxNum, RAND_MAX)>
///////////////////////////////////////////////////////////////////
int rand(int maxNum) { return rand() % (maxNum + 1); }

///////////////////////////////////////////////////////////////////
// doKeyboard
// GLUT window sub-API
///////////////////////////////////////////////////////////////////
void doKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27 : exit(0);
        case '1' : s_fps = aminmax(0, s_fps - 1, 1000); break;
        case '2' : s_fps = aminmax(0, s_fps + 1, 1000); break;
        case '3' : s_bps = aminmax(0, s_bps - 1, 1000); break;
        case '4' : s_bps = aminmax(0, s_bps + 1, 1000); break;
        case '5' : s_speed = aminmax(0.0f, s_speed - 0.01f, 1.0f); g_game->m_time->setSpeed(s_speed); break;
        case '6' : s_speed = aminmax(0.0f, s_speed + 0.01f, 1.0f); g_game->m_time->setSpeed(s_speed); break;
        case '0' : delete g_game; g_game = new CGame(70 + rand(100), 15 + rand(10), 40 + rand(40), rand(2)); g_game->m_time->setSpeed(s_speed); break;
    }
}

///////////////////////////////////////////////////////////////////
// doDisplay
// GLUT window sub-API; currently empty as all drawing is done in doLoop
///////////////////////////////////////////////////////////////////
void doDisplay() {}

///////////////////////////////////////////////////////////////////
// doLoop
// GLUT window sub-API. main simulation loop
///////////////////////////////////////////////////////////////////
void doLoop() {
    TRealTime now = g_game->m_time->timeNow();
    if (g_game->getDesiredFramesDone(now, s_fps) > g_game->getRenderedSinceSecond()) {
        g_game->m_periodTimer.startPeriod(PERIOD_RENDER);
        g_game->newFrame();
        g_game->buildFrameConstant();
        g_game->renderFrame();
        g_game->m_periodTimer.startPeriod(PERIOD_IDLE);
    }
    if (g_game->getDesiredFramesDone(now, s_bps) > g_game->getBuiltSinceSecond()) {
        g_game->m_periodTimer.startPeriod(PERIOD_BUILD);
        g_game->newBuild();
        g_game->buildFramePeriodic();
        g_game->m_periodTimer.startPeriod(PERIOD_IDLE);
    }
    g_game->updateFPS(now);
}


///////////////////////////////////////////////////////////////////
// Main program entry point. initializes the game and GLUT window.
///////////////////////////////////////////////////////////////////
#ifdef WIN32
int pascal WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR, int cmdShow) {
#else
int main(int argc, char* argv[]) {
#endif
    g_game = new CGame(WORLD_GRANULARITY, WORLD_PEAKS, PREY_NUM, PREDATOR_NUM);
    g_game->m_time->setSpeed(s_speed);

    glutInit(&argc, argv);
    gfxInit();
    glutDisplayFunc(doDisplay);
    glutIdleFunc(doLoop);
    glutMouseFunc(doMouseButtons);
    glutMotionFunc(doMouseMove);
    glutPassiveMotionFunc(doMouseMove);
    glutKeyboardFunc(doKeyboard);

    glutMainLoop();

    delete g_game; // automatically destroys all registered agents
}

