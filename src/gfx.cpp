////////////////////////////////////////////////////////////////////////////////
// file: gfx.cpp
// author: Radim Rehurek
// last update: 29.09.04
//
// anything to do with graphics goes here
//////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdarg>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "world.h"
#include "game.h"
#include "prey.h"
#include "predator.h"
#include "demo.h"

const float PREDATOR_SIZE = 0.005f; // predator size (in world units)
const float PREY_SIZE = 0.003f; // prey size

RECT clRect = {150, 50, 850, 850}; // initial window screen coordinates

int PREDATOR_GLLIST;
int PREY_GLLIST;

// define some material properties
GLfloat mat_specular2[] = {0.508273f, 0.508273f, 0.508373f};
GLfloat mat_ambient2[] = {0.19225f, 0.19225f, 0.19225f};
GLfloat mat_diffuse2[] = {0.50754f, 0.50754f, 0.50754f};
GLfloat mat_shininess2[] = {128.0f * 0.8f};

///////////////////////////////////////////////////////////////////
// CWorld::drawWorld
// draws the cells naively one after another; plenty of room for optimizations here
///////////////////////////////////////////////////////////////////
void CWorld::drawWorld() const {
    double over = 1.0 / m_gridSize;
    double transl_y = 0.0;
    int index = 0;
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    for (int i = 0; i < m_gridSize; i++, transl_y += over) {
        double transl_x = 0.0;
        for (int j = 0; j < m_gridSize; j++, transl_x += over, ++index) {
            CGridCell const *cell = cellAt(index);
            glColor3f(0.39f, 0.039f + cell->m_grass * 0.78f, 0.039f);
            glVertex3f(transl_x, cell->m_height[0], transl_y);
            glVertex3f(transl_x + over, cell->m_height[1], transl_y);
            glVertex3f(transl_x + over, cell->m_height[2], transl_y + over);
            glVertex3f(transl_x, cell->m_height[3], transl_y + over);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////
// CPredator::drawAgent
// draws a sphere as compiled gl list
///////////////////////////////////////////////////////////////////
void CPredator::drawAgent() const {
    glPushMatrix(); {
        glTranslatef(getPosX(), PREDATOR_SIZE + getAgentHeight(), getPosY());
        if (this->m_active)
            glColor3f(1.0f, 0.3f, 0.3f);
        else
            glColor3f(0.1f, 0.0f, 0.0f);
        glCallList(PREDATOR_GLLIST);
    } glPopMatrix();
}

///////////////////////////////////////////////////////////////////
// CPrey::drawAgent
// draws a sphere as compiled gl list
///////////////////////////////////////////////////////////////////
void CPrey::drawAgent() const {
    glPushMatrix(); {
        glTranslatef(getPosX(), PREY_SIZE + getAgentHeight(), getPosY());
        if (this->m_active)
            glColor3f(0.3f, 0.3f, 1.0f);
        else
            glColor3f(0.1f, 0.0f, 0.0f);
        glCallList(PREY_GLLIST);
    } glPopMatrix();
}

///////////////////////////////////////////////////////////////////
// mouse input parameters
///////////////////////////////////////////////////////////////////
const float MOUSE_XSENS = 0.004f;
const float MOUSE_YSENS = 0.005f;
const float MOUSE_ZOOMSENS = 0.003f;

float cam_rotx = M_PIf / 8;
float cam_roty = 0.0f;
float cam_distance = 0.9f;
Vector2d cam_pos(0.5f, 0.5f);
ECameraState mouseState = CAMERA_NONE;
Vector2d mouseLast;

///////////////////////////////////////////////////////////////////
// doMouseMove
// GLUT window API routine
///////////////////////////////////////////////////////////////////
void doMouseMove(int x, int y) {
    Vector2d mouseNow(x, y);
    Vector2d mouseDiff = mouseNow - mouseLast;
    switch ( mouseState ) {
        case CAMERA_ROTATE:
            {
                cam_roty = cam_roty - MOUSE_XSENS * mouseDiff.m_x;
                cam_rotx = aminmax(M_PIf / 8, cam_rotx - MOUSE_YSENS * mouseDiff.m_y, M_PIf / 2 - 0.0001f);
            }
            break;
        case CAMERA_SCROLL:
            {
                Vector2d forward(sin(cam_roty) * mouseDiff.m_y, cos(cam_roty) * mouseDiff.m_y);
                Vector2d strafe(cos(cam_roty) * mouseDiff.m_x, -sin(cam_roty) * mouseDiff.m_x);
                Vector2d offset = forward + strafe;
                cam_pos = cam_pos + (offset * 0.003f);
                cam_pos.clamp(Vector2d(0.0f, 0.0f), Vector2d(1.0f, 1.0f));
            }
            break;
        case CAMERA_ZOOM:
            {
                cam_distance = aminmax(0.1f, cam_distance - MOUSE_ZOOMSENS * mouseDiff.m_y, 1.1f);
            }
            break;
        default:
            mouseState = CAMERA_ROTATE;
            break;
    }
    int middx = (clRect.right - clRect.left) / 2;
    int middy = (clRect.bottom - clRect.top) / 2;
    const int maxdist = 20;
    if (x < middx - maxdist || x > middx + maxdist || y < middy - maxdist || y > middy + maxdist) {
        x = middx; y = middy;
        glutWarpPointer(middx, middy);
        //SetCursorPos(x + clRect.left, y + clRect.top);
    }
    mouseLast = Vector2d(x, y);
}

///////////////////////////////////////////////////////////////////
// doMouseButtons
// GLUT window API routine
///////////////////////////////////////////////////////////////////
void doMouseButtons(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
         if (button == GLUT_LEFT_BUTTON) {
            mouseState = CAMERA_ZOOM;
        }
    } else if (state == GLUT_UP) {
         if (button == GLUT_LEFT_BUTTON) {
            mouseState = CAMERA_ROTATE;
        }
    }
}

///////////////////////////////////////////////////////////////////
// setCamera
///////////////////////////////////////////////////////////////////
void setCamera() {
    Vector3d target = Vector3d(cam_pos.m_x, g_game->m_world->getHeight(cam_pos), cam_pos.m_y);
    Vector3d eye(0.0f, 0.0f, cam_distance);
    eye.rotate(cam_rotx, cam_roty, 0.0f);
    eye.translate(target);
    g_game->m_camera->setTarget(target);
    g_game->m_camera->setPosition(eye);
    g_game->m_camera->setFOV(100.0f);
}

///////////////////////////////////////////////////////////////////
// outText
// prints text at specified position as bitmap characters
///////////////////////////////////////////////////////////////////
void outText(int x, int y, const char * text) {
    glRasterPos2f(x, y);
    for (const char * ch = text; *ch; ch++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
}

void printText(int x, int y, char *format, ...) {
    va_list va;
    char buf[256];
    va_start(va, format);
    vsprintf(buf, format, va);
    va_end(va);
    outText(x, y, buf);
}

///////////////////////////////////////////////////////////////////
// CGame::renderFrame
// draw the world, draw the agents. also prints some stats
///////////////////////////////////////////////////////////////////
void CGame::renderFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable (GL_LIGHTING);
    glColor3f(0.7f, 0.0f, 1.0f);
    //printText(10, 40, "%.4g fps / %.4g fps", m_framesRenderedPS, m_framesBuiltPS);
    printText(10, 40, "Rendered : %.1f fps (load %.0f%%)", m_framesRenderedPS, 100 * m_periodTimer.getTimeSpent(PERIOD_RENDER));
    printText(10, 64, "Built : %.1f fps (load %.0f%%)", m_framesBuiltPS, 100 * m_periodTimer.getTimeSpent(PERIOD_BUILD));
    if (100 * m_periodTimer.getTimeSpent(PERIOD_IDLE) < 10)
        glColor3f(1.0f, 0.2f, 0.2f);
    printText(10, 88, "Idle (load %.0f%%)", 100 * m_periodTimer.getTimeSpent(PERIOD_IDLE));
    glColor3f(0.3f, 0.4f, 0.5f);
    printText(10, 120, "desired %d fps / %d bps", s_fps, s_bps);
    printText(10, 144, "simulation speed %.2f / time %.3f", m_time->getSpeed(), m_time->tickNow());
    printText(10, 168, (char *)(g_game->m_time->getHWCounterPresent() ? "hw counter" : "sw counter"));
    glEnable(GL_LIGHTING);

    setCamera();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); {
        glLoadIdentity();
        gluPerspective(m_camera->getFOV() / 2, m_camera->getAspectRatio(), 0.001, 2.0 );

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); {
            glLoadIdentity();
            gluLookAt(
                m_camera->getPosition().getX(), m_camera->getPosition().getY(), m_camera->getPosition().getZ(),
                m_camera->getTarget().getX(), m_camera->getTarget().getY(), m_camera->getTarget().getZ(),
                m_camera->getUpVector().getX(), m_camera->getUpVector().getY(), m_camera->getUpVector().getZ());

            for (std::list <CAgent *>::iterator agent = m_agents.begin(); agent != m_agents.end(); agent++) {
                (*agent)->drawAgent();
                (*agent)->m_marked = false;
            }
            m_world->drawWorld();
        } glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    } glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////
// setMaterial
// specify material parameters
///////////////////////////////////////////////////////////////////
void setMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[]) {
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
  glMaterialfv(GL_FRONT, GL_SHININESS, shin);
  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
}

///////////////////////////////////////////////////////////////////
// gfxInit
// initialize graphics. called once at program start
///////////////////////////////////////////////////////////////////
void gfxInit() {
    int w = clRect.right - clRect.left + 1;
    int h = clRect.bottom - clRect.top + 1;
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(clRect.left, clRect.top);
    glutCreateWindow("Predator&Prey");

    glMatrixMode(GL_PROJECTION); // for bitmap fonts..
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScalef(1, -1, 1);
    glTranslatef(0, -h, 0);
    glMatrixMode(GL_MODELVIEW);

    glutSetCursor(GLUT_CURSOR_NONE);
    doMouseMove(0, 0);

    GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0);
    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    PREDATOR_GLLIST = glGenLists(1);
    glNewList(PREDATOR_GLLIST, GL_COMPILE); {
        glutSolidSphere(PREDATOR_SIZE, 10, 10);
    } glEndList();
    PREY_GLLIST = glGenLists(1);
    glNewList(PREY_GLLIST, GL_COMPILE); {
        glutSolidSphere(PREY_SIZE, 8, 8);
    } glEndList();
    setMaterial(mat_specular2, mat_ambient2, mat_diffuse2, mat_shininess2);
    //glutFullScreen();
}
