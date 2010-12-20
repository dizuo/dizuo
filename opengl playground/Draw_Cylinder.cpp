#include <gl/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "Cylinder.h"

void	init(void);
void	reshape(int w,int h);
void	display(void);

Cylinder	g_cylinder(20, 100, 10, 3);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow("Sphere");
	init ();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

void init (void)
{ 
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth(1);
	glShadeModel(GL_SMOOTH);
	GLfloat _ambient[]={1.0,1.0,1.0,1.0};
	GLfloat _diffuse[]={1.0,1.0,0.0,1.0};
	GLfloat _specular[]={1.0,1.0,1.0,1.0};
	GLfloat _position[]={200,200,200,0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,_position);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500, 0.0, 500, -500, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void display(void)
{
	static double angleX = 0.0;
	static double angleY = 0.0;
	static double angleZ = 0.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glTranslated(100, 100, 0);

	glRotated(angleX, 1,0,0);
	//glRotated(angleY, 0,1,0);
	//glRotated(angleZ, 0,0,1);
	glColor3f(1.0,1.0,1.0);

	g_cylinder.Render( SOLID );
	
	glFlush();
	angleX += 10;
	//angleY += 10;
	//angleZ += 10;
}

