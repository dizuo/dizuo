#include "glut.h"
#include "draw.h"
#include "Keyboard.h"
#include "Camera3d.h"

glut::glut(void)
{
}

glut::~glut(void)
{
}

void glut::registraDraw(void)
{ 
	glutDisplayFunc(draw::desenha);

}
void glut::init(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutCreateWindow("TeaPot");         
}

void glut::registraKeyboard()
{
	glutKeyboardFunc(Keyboard::Teclado);
}
void glut::registraReshape()
{
}

void glut::loop()
{
	glutMainLoop();
}

void glut::setViewPort(int a, int b, int c, int d)
{
	glViewport(a, b,c,d);
}

void glut::setCamera()
{
	Camera3d cam; 
	cam.setLentes(45,1);
	cam.setLookAt(0,160,400, 0,0,0, 0,1,0);
}