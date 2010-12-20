#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <ctime>

#include "ColorTable.h"

//	12/9/2010 RYF
//	QQ：546952720
//	Email: dizuo@126.com

//////////////////////////////////////////////////

float	g_fWidth = 800;
float	g_fHeight = 600;
float	g_fDepth = 100;
float	g_fAngle = .0;

//////////////////////////////////////////////////////////////////////////
void	renderBitmapString2D(float x, float y, 
							char *str, 
							void *font=GLUT_BITMAP_TIMES_ROMAN_24);


void init(void) 
{
	glClearColor (0., 0., 0., 0.0);
	glShadeModel (GL_SMOOTH);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, g_fWidth, 0, g_fHeight, 0, g_fDepth);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 将窗口坐标系置于窗口中心位置
	glTranslatef(g_fWidth/2.0f, g_fHeight/2.0f, 0.f);		
	
	renderBitmapString2D(0, 0, "wheel");

	glutSwapBuffers();
	
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60,1.0,1.5,20);
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON)
		switch(state)
	{
		case GLUT_DOWN:
			break;

		case GLUT_UP:
			break;
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMouseFunc( mouse );
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);

	glutMainLoop();
	return 0;
}

void renderBitmapString2D(float x, float y, char *str, void *font)
{
	int i = sizeof(str);

	int fontWidOffset = i>1 ? 6*i : 5;		//估算量
	int fontHighOffset = 10;					//估算量

	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x-fontWidOffset, y-fontHighOffset);
	// loop all the characters in the string
	for (c=str; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
