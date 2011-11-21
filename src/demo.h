#ifndef _demo_h_
#define _demo_h_

#define WIN32_LEAN_AND_MEAN

#include <math.h>
#ifdef WIN32
#include <windows.h>
#else
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;

typedef struct _RECT {
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT, *PRECT;

#endif

#define M_PIf 3.14159265358979323846f

extern float randf();
extern int rand(int max);
extern void doMouseMove(int x, int y);
extern void doMouseButtons(int button, int state, int x, int y);
extern void gfxInit();
extern int s_fps; // desired frame rate
extern int s_bps; // desired build rate
extern RECT clRect;

template <class T> inline T aminmax(T low, T val, T high) {
    if (val < low)
        return low;
    if (val > high)
        return high;
    return val;
}

enum ECameraState {
    CAMERA_NONE = 0,
    CAMERA_ROTATE,
    CAMERA_SCROLL,
    CAMERA_ZOOM
};

enum EPeriod {
    PERIOD_IDLE = 0,
    PERIOD_BUILD,
    PERIOD_RENDER,
    PERIOD_NUM // must stay last
};

struct Vector2d {
public:
    float m_x;
    float m_y;
    Vector2d(float x = 0, float y = 0) : m_x(x), m_y(y) {}
    Vector2d(const Vector2d &v) { m_x = v.m_x; m_y = v.m_y; }
    void scale(float factor) { m_x *= factor; m_y *= factor; }
    void stretchTo(float len) { float length = this->length(); if (length > 0) this->scale(len / length); }
    void normalize() { this->stretchTo(1.0f); }
    void truncate(float len) { float length = this->length(); if (length > len) this->stretchTo(len); }
    void clamp(const Vector2d &p1, const Vector2d &p2) { m_x = aminmax(p1.m_x, m_x, p2.m_x); m_y = aminmax(p1.m_y, m_y, p2.m_y); }

    float sqrLength() const { return m_x * m_x + m_y * m_y; }
    float length() const { return sqrtf(m_x * m_x + m_y * m_y); }
    Vector2d operator+(const Vector2d &v2) const { return Vector2d(m_x + v2.m_x, m_y + v2.m_y); }
    Vector2d operator-() const { return Vector2d(-m_x, -m_y);	}
    Vector2d operator-(const Vector2d &v2) const { return (*this) + (-v2); }
    Vector2d operator*(float f) const { return Vector2d(m_x * f, m_y * f); }
};

class Vector3d {
private:
    enum {X = 0, Y, Z};
    float m_vector[3];
public:
    Vector3d(float x = 0, float y = 0, float z = 0) { set(x, y, z); }
    Vector3d(const Vector3d &v) { set(v.getX(), v.getY(), v.getZ()); }
    float getX() const { return m_vector[X]; }
    float getY() const { return m_vector[Y]; }
    float getZ() const { return m_vector[Z]; }
    float get(int ind) { return m_vector[ind]; }
    void setX(float x) { m_vector[X] = x; }
    void setY(float y) { m_vector[Y] = y; }
    void setZ(float z) { m_vector[Z] = z; }
    void set(int ind, float v = 0) { m_vector[ind] = v; }
    void set(float x, float y, float z) { setX(x); setY(y); setZ(z); }
    Vector3d operator+(const Vector3d &v2) const { return Vector3d(getX() + v2.getX(), getY() + v2.getY(), getZ() + v2.getZ()); }
    Vector3d operator-() const { return Vector3d(-getX(), -getY(), -getZ()); }
    Vector3d operator-(const Vector3d &v2) const { return (*this) + (-v2); }
    Vector3d operator*(float f) const { return Vector3d(getX() * f, getY() * f, getZ() * f); }
    void rotateX(float angX) { // FIXME introduce matrices?
        float cx = cosf(angX);
        float sx = sinf(angX);
        float a = cx * getY() + sx * getZ();
        float b = -sx * getY() + cx * getZ();
        setY(a);
        setZ(b);
    }
    void rotateY(float angY) {
        float cx = cosf(angY);
        float sx = sinf(angY);
        float a = cx * getX() + sx * getZ();
        float b = -sx * getX() + cx * getZ();
        setX(a);
        setZ(b);
    }
    void rotateZ(float angZ) {
        float cx = cosf(angZ);
        float sx = sinf(angZ);
        float a = cx * getX() + sx * getX();
        float b = -sx * getY() + cx * getY();
        setX(a);
        setY(b);
    }
    void rotate(float rotx, float roty, float rotz) {
        rotateX(rotx);
        rotateY(roty);
        rotateZ(rotz);
    }
    void translate(float x, float y, float z) {	set(getX() + x, getY() + y, getZ() + z); }
    void translate(const Vector3d &v) { set(getX() + v.getX(), getY() + v.getY(), getZ() + v.getZ()); };
};


#endif
