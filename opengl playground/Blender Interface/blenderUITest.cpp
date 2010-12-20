#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <ctime>

//#include "BezierLinkRender.h"
#include "RoundBoxRender.h"

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

float g_fWidth = 500;
float g_fHeight = 500;
float g_fDepth = 100;
float g_fAngle = .0;

int		gRoundboxtype = 0;
//////////////////////////////////////////////////////////////////////////
void renderBitmapString2D(float x, float y, void *font, char *string);
void getColorFromTable(GLuint rgb, float vec[3]);

void init(void) 
{
	RoundBoxRender::setType(gRoundboxtype);
	printf("%d\n", gRoundboxtype);

	glClearColor (0.9, 0.9, 0.9, 0.0);
	glShadeModel (GL_SMOOTH);
}

void display(void)
{
	const float slotSize = 30.f;
	const float slotColor[] = {0.8f, 0.8f, 0.8f};
	const float slotShadowSize = 2.0f;
	const float slotShadowAlpha = 0.7;

	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, g_fWidth, 0, g_fHeight, 0, g_fDepth);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(80.f, 80.f, 0.f);		//整体平移
	
	int xPos(0);
	int yPos(0);
	int xOffset(0);
	int yOffset(0);

	for (int i=0; i<16; i++)
	{
		char buffer[10] = {'\0'};
		itoa (i,buffer,10);

		int fontWidOffset = i>10 ? 12 : 5;		//估算量
		int fontHghOffset = 10;					//估算量

		int boxSize = 60;
		
		float colorVec[3];
		getColorFromTable(gColorStable[i], colorVec);
		glColor3fv(colorVec);
		RoundBoxRender::setType(i);

		yOffset = i/4;
		xOffset = i%4;

		glPushMatrix();
		{
			glTranslatef(xPos+xOffset*100, yPos+yOffset*100, 0.f);
			RoundBoxRender::gl_round_box_shade(GL_POLYGON, 0, 0, boxSize, boxSize, 10, 0.3, 0);
			glColor3f(0.f, 0.f, 0.0f);
			renderBitmapString2D(boxSize/2-fontWidOffset, boxSize/2-fontHghOffset, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		}glPopMatrix();
	}
	//glColor3f(1.0f, 1.0f, 0.0f);
	//ui_draw_link_bezier(&gRect);

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
	glutMainLoop();
	return 0;
}

void renderBitmapString2D(float x, float y, void *font, char *string)
{
	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x, y);
	// loop all the characters in the string
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
void getColorFromTable(GLuint rgb, float vec[3])
{
	GLubyte  r = GLubyte ((rgb >> 16) & 0xFF);
	GLubyte  g = GLubyte ((rgb >> 8) & 0xFF);
	GLubyte  b = GLubyte (rgb & 0xFF);
	vec[0] = r / 255.0f;
	vec[1] = g / 255.0f;
	vec[2] = b / 255.0f;
}