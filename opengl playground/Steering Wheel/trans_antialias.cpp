#include <cstdio>
#include <cstdlib>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <cmath>
#include <ctime>

//	7/26/2010 RYF
//	QQ：546952720
//	Email: dizuo@126.com
//  点线抗锯齿
//  多边形抗锯齿不行

#include "CircleRender.h"

//////////////////////////////////////////////////
//global
GLuint gColorStable[] = 
{
	(0xFF0000),	//red 
	(0xDC143C),	//crimson
	(0x9400D3),	//violet
	(0xFFA000),	//orange
	(0xFFFF00),	//yellow
	(0xFFD700),	//glod
	(0x00FF00),	//green
	(0x90FF90),	//lightgreen
	(0x006400),	//darkgreen
	(0x556B2F),	//olivegreen		//very good color绿橄榄
	(0x0000FF),	//blue
	(0x00008B),	//darkblue
	(0x4169E1),	//royalblue
	(0x5555FF),	//skyblue
	(0x191970),	//midnightbue
	(0xFF00FF),	//fuchsia
	(0x00FFFF),	//aqua
	(0xffb6c1),	//pink
	(0xFF91A4),	//salmonpink
	(0x30D5C8),	//turquoise
	(0x008080),	//darkturpuoise
	(0xA9A9A9),	//gray
	(0xD3D3D3),	//lightgray
	(0x808080)	//darkgray
};
	
float	g_fWidth = 500;
float	g_fHeight = 500;
float	g_fDepth = 100;
float	g_fAngle = .0;

//////////////////////////////////////////////////////////////////////////
void	renderBitmapString2D(float x, float y, 
							char *str, 
							void *font=GLUT_BITMAP_TIMES_ROMAN_24);

void	getColorFromTable(GLuint rgb, float a, float vec[4]);

void init(void) 
{
	glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

	// enable /disable features
	// glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable (GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glLineWidth(5.0f);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glClearStencil(0);                              // clear stencil buffer
	glClearDepth(1.0f);                             // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
	
	glTranslatef(-75.0f, 0.0f, 0.0f);
	//////////////////////////////////////////////////////////////////////////
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float angle = 0.0f;
	float color[4];

	getColorFromTable(0x0000FF, 0.33f, color);
	glColor4fv(color);
	CircleRender::render_circle_area(75.0f, 100.0f, 0.f, 100, 32);
	//renderBitmapString2D(0, 0, "wheel");

	getColorFromTable(0xFF0000, 0.33f, color);
	glColor4fv(color);
	CircleRender::render_circle_area(150.0f, 0.0f, 0.f, 100, 32);

	getColorFromTable(0x00FF00, 0.33f, color);
	glColor4fv(color);
	CircleRender::render_circle_area(0.0f, 0.0f, 0.0f, 100, 32);
	//glPushMatrix();
	//glRotatef(g_fAngle, 0.0, 0.0, 0.1);
	//glColor4fv(color1);
	//glBegin(GL_TRIANGLES);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(-100.0f, -50.0f, 0.0f);
	//glVertex3f(100.0f, -50.0f, 0.0f);
	//glVertex3f(.0f, 100.0f, 0.0f);
	//glEnd();
	//glPopMatrix();

	// line antialias ok
	//glPushMatrix();
	//glRotatef(g_fAngle, 0.0, 0.0, 0.1);
	//glColor4fv(color1);
	//glBegin(GL_LINES);
	//glVertex3f(-100.0f, -50.0f, 0.0f);
	//glVertex3f(100.0f, -50.0f, 0.0f);
	//glEnd();
	//glPopMatrix();

	//////////////////////////////////////////////////////////////////////////
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
	case 'r':
	case 'R':
		g_fAngle += 1.0;
		if (g_fAngle >= 360.) g_fAngle = 0.;
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
	// 需要将鼠标位置变换到当前坐标系：窗口中心为原点，x右，y上
	int x_window = x - g_fWidth/2;
	int y_window = g_fHeight/2 - y;

	//printf("%i \t %i\n", x_window, y_window);

	//glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
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

void getColorFromTable(GLuint rgb, float a, float vec[4])
{
	GLubyte  r = GLubyte ((rgb >> 16) & 0xFF);
	GLubyte  g = GLubyte ((rgb >> 8) & 0xFF);
	GLubyte  b = GLubyte (rgb & 0xFF);
	vec[0] = r / 255.0f;
	vec[1] = g / 255.0f;
	vec[2] = b / 255.0f;

	a = a>1.0f ? 1.0 : a;
	a = a<0.0f ? 0.0 : a;

	vec[3] = a;
}
