#include "Camera2d.h"
#include <gl/glut.h>

Camera2d::Camera2d(void)
{
}

Camera2d::~Camera2d(void)
{
}

void Camera2d::setLentes(int left,int right, int bottom, int top)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
}
