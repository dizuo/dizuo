#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <ctime>

#include "Sensor_concept.h"

//	7/9/2010 RYF
//	QQ：546952720
//	Email: dizuo@126.com

//////////////////////////////////////////////////////////////////////////
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
class Rect : public Shape
{
public:
	bool contains(int x, int y)
	{
		return true;
	}
	
	void doDeactive()
	{
		isActive = false;
	}

	void doActive()
	{
		isActive = true;
	}

	void render()
	{
		//if (isActive)
		//	
		//glRectf(x1, y1, x2, y2);
	}

private:
	float x1;
	float y1;
	float x2;
	float y2;

	bool isActive;
};
void	renderBitmapString2D(float x, float y, 
							char *str, 
							void *font=GLUT_BITMAP_TIMES_ROMAN_24);

void	getColorFromTable(GLuint rgb, float vec[3]);

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
	
	float color[3];
	getColorFromTable(0x556B2F, color);
	glColor3fv(color);
	glRectf(0, 0, 100, 100);

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
	// 需要将鼠标位置变换到当前坐标系：窗口中心为原点，x右，y上
	int x_window = x - g_fWidth/2;
	int y_window = g_fHeight/2 - y;

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

void getColorFromTable(GLuint rgb, float vec[3])
{
	GLubyte  r = GLubyte ((rgb >> 16) & 0xFF);
	GLubyte  g = GLubyte ((rgb >> 8) & 0xFF);
	GLubyte  b = GLubyte (rgb & 0xFF);
	vec[0] = r / 255.0f;
	vec[1] = g / 255.0f;
	vec[2] = b / 255.0f;
}
