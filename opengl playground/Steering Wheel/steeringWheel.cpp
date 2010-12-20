#include <cstdio>
#include <cstdlib>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <cmath>
#include <ctime>

//	7/9/2010 RYF
//	QQ：546952720
//	Email: dizuo@126.com

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

// 扇形是区间的概念~
//	 1，5个角度值，非循环构成了四个区间，定义了四个扇形。
//	 2，也可以四个角度值，循环构成四个区间，定义四个扇形。
//	这里采用第二种
//	所有的扇形角度值都位于[0, 2*M_PI]

const int	gSectorNum = 12;
bool		gSectorStatus[gSectorNum] = {0};
float		gSectorAngles[gSectorNum];
	
float	g_fWidth = 500;
float	g_fHeight = 500;
float	g_fDepth = 100;
float	g_fAngle = .0;

//////////////////////////////////////////////////////////////////////////
void	renderBitmapString2D(float x, float y, 
							char *str, 
							void *font=GLUT_BITMAP_TIMES_ROMAN_24);

void	getColorFromTable(GLuint rgb, float vec[3]);
int		getCircleSectorByPoint(float r, 
							  float point_x, float point_y);

void init(void) 
{
	glClearColor (0., 0., 0., 0.0);
	glShadeModel (GL_SMOOTH);

	const float deltaAngle = 360.0 / gSectorNum;
	for (int i(0); i<gSectorNum; i++)
		//gSectorAngles[i] = 45.0f + i*90.0f;
		gSectorAngles[i] = deltaAngle*i;

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
	
	float angle = 0.0f;
	float color[3];

	//getColorFromTable(0xD3D3D3, color);
	//glColor3fv(color);
	//CircleRender::render_circle_area(0.f, 0.f, 0.f, 101);		// draw gray circle in the center

	//glColor3f(1.0, 1.0, 0.5);
	//CircleRender::render_circular_sector_area(100.0f, 200.0f, 315, 405);

	for (int i=0; i<gSectorNum; i++)
	{
		int next = (i+1)%gSectorNum;

		float delta(0);
		if (gSectorStatus[i])
		{
			delta = 10;
			//getColorFromTable(0xD3D3D3, color);
			//getColorFromTable(0xC0C0C0, color);
		}
		else
			getColorFromTable(gColorStable[i], color);
		
		glColor3fv(color);

		CircleRender::render_circular_sector_area(100.0f-delta, 200.0f+delta, 
												gSectorAngles[i], gSectorAngles[next]);
	}
	getColorFromTable(0x5555FF, color);
	glColor3fv(color);
	renderBitmapString2D(0, 0, "wheel");

	glutSwapBuffers();
	
	for (int i=0; i<gSectorNum; i++)	gSectorStatus[i]=false;
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

	int id1 = getCircleSectorByPoint(200.0f, x_window, y_window);
	int id2 = getCircleSectorByPoint(100.0f, x_window, y_window);
	
	if (id1==id2)
		return;

	if (id1>-1 && -1==id2)
	{
		//printf("OK~~~~~~~~\n");
		gSectorStatus[id1] = true;
	}

	//printf("%i \t %i\n", x_window, y_window);

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	
	GLint bufs, samples;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
	glGetIntegerv(GL_SAMPLES, &samples);

	if (bufs==1 && samples>1)
		printf("multi sample is sucess! \n");
	else
		printf("multi sample is failed! \n");

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

/*
 * circle圆心是原点
 * 始终还是有问题：第四象限到第一象限
 * 下面两种扇形：
 * [315 - 45]
 * [315 - 405]
 */
int getCircleSectorByPoint(float r, 
						   float point_x, float point_y)
{
	if ( std::sqrt(point_x*point_x + point_y*point_y) > r )
		return -1;

	float point_rad = std::atan(point_y/point_x);

	// 将point_rad根据point的坐标变换到四个象限中
	
	bool sign1 = point_x*point_y > 0.0f;
	bool sign2 = point_x > 0.0f;

	//sign1 && sign2		// first quadrant 第一象限 
	//sign1 && !sign2		// third quadrant 第三象限
	//!sign1 && sign2		// fourth quadrant 第四象限
	//!sign1 && !sign2		// second quadrant 第二象限

	if ( (sign1 && !sign2) 
		 || (!sign1 && !sign2) )
		point_rad += M_PI;
	else if (!sign1 && sign2)	
		point_rad += 2*M_PI;

	// 0 <= point_rad <= 2*M_PI
	printf("%f\n", point_rad);

	// 在gSectorAngles数组中查找位置
	
	//return point_rad > beg_rad && point_rad < end_rad;

	int sectorIndex(0);
	for (; sectorIndex<gSectorNum; sectorIndex++)
	{
		float sector_rad = gSectorAngles[sectorIndex]*M_PI/180.f;
		if (sector_rad>point_rad)
			break;
	}

	return sectorIndex==0 ? gSectorNum-1 : sectorIndex-1;
}