#include "Camera3d.h"
#include <gl/glut.h>

Camera3d::Camera3d(void)
{
}

Camera3d::~Camera3d(void)
{
}

void Camera3d::setLentes(int angle, int ratio)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle,ratio,0.2,500);

}

void Camera3d::setLookAt(float ox,float oy,float oz,float dx,float dy,float dz,float ux,float uy,float uz)
{
	gluLookAt(ox,oy,oy, dx,dy,dz,ux ,uy,uz);
}
