#include <iostream>
#include <vector>
#include <GL/glut.h>

#include "BezierCurve.h"

using namespace std;

enum WindowSize{
	WinWidth = 1024,
	WinHeight = 768
};

int			g_Viewport[4];
double		g_ModelMatrix[16];
double		g_ProjMatrix[16];
BezierCurve myBezier;

//////////////////////////////////////////////////////////////////////////
void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (WinWidth, WinHeight);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);

	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	myBezier.begin();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 0.0);
	glPointSize(5.0);

	glPushMatrix();
	{
		myBezier.renderCurve();
	}glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
		5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
	else
		glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
		5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  case 27:
		  exit(0);
		  break;
	}
}

void mouse(int button, int state, int x, int y)
{
	double vertex[3];

	//获取矩阵信息
	glGetIntegerv(GL_VIEWPORT, g_Viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, g_ModelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, g_ProjMatrix);

	y = g_Viewport[3] - y;
	gluUnProject( x, y, 0,
		g_ModelMatrix, g_ProjMatrix, g_Viewport,
		&vertex[0], &vertex[1], &vertex[2] );

	if (button==GLUT_LEFT && state==GLUT_DOWN)
	{
		myBezier.mouseSynchro( BezierCurve::LButtonDown, vertex );
		glutSetCursor( GLUT_CURSOR_RIGHT_ARROW );
	}
	else if (button == GLUT_LEFT && state == GLUT_UP)
	{
		myBezier.mouseSynchro( BezierCurve::LButtonUp, vertex );
	}

	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////
// 计算控制节点
void motion(int x, int y)
{
	double vertex[3];

	glutSetCursor( GLUT_CURSOR_CROSSHAIR );
	y = g_Viewport[3] - y;

	gluUnProject( x, y, 0,
		g_ModelMatrix, g_ProjMatrix, g_Viewport,
		&vertex[0], &vertex[1], &vertex[2] );

	myBezier.mouseSynchro( BezierCurve::MouseMove, vertex );
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////
//  [9/27/2009 Bible]
//	组合类