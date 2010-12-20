#include <iostream>
#include <vector>
#include <GL/glut.h>

using namespace std;

#define KEY_POINT_NUM 6
#define CONTROL_POINT_NUM 4
#define WinWidth	1024
#define WinHeight	768

#define DrawOneLine(x1, y1, z1, x2, y2, z2) glBegin(GL_LINES);	\
	glVertex3d( (x1), (y1), (z1) ); glVertex3d( (x2), (y2), (z2) ); glEnd(); 

enum	DRAW_FLAG{
		FIRST_LINE = 0,
		SECOND_LINE = 1,
		END = 2
};

int		g_iFlag = FIRST_LINE;
int		g_Viewport[4];
bool	g_bIsDown = false;
double	g_ModelMatrix[16];
double	g_ProjMatrix[16];
double  g_Vertex[KEY_POINT_NUM][3];	//6个关键点，其中四个为控制结点。
double	g_ControlVertex[CONTROL_POINT_NUM+3][3];

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
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5.0);
	glColor3f(1.0, 1.0, 0.0);

	glEnable(GL_LINE_STIPPLE);
	{
		glLineStipple(1, 0x0101);
		if (FIRST_LINE==g_iFlag)
		{
			DrawOneLine( g_Vertex[0][0], g_Vertex[0][1], g_Vertex[0][2], 
						 g_Vertex[1][0], g_Vertex[1][1], g_Vertex[1][2] );
			DrawOneLine( g_Vertex[0][0], g_Vertex[0][1], g_Vertex[0][2], 
		   				 g_Vertex[2][0], g_Vertex[2][1], g_Vertex[2][2] );

			glBegin(GL_POINTS);
			{
				glVertex3dv(g_Vertex[0]);
				glVertex3dv(g_Vertex[1]);
				glVertex3dv(g_Vertex[2]);
			} glEnd();
		}
		else if (SECOND_LINE==g_iFlag)
		{
			DrawOneLine( g_Vertex[3][0], g_Vertex[3][1], g_Vertex[3][2], 
						 g_Vertex[4][0], g_Vertex[4][1], g_Vertex[4][2] );
			DrawOneLine( g_Vertex[3][0], g_Vertex[3][1], g_Vertex[3][2], 
						 g_Vertex[5][0], g_Vertex[5][1], g_Vertex[5][2] );

			glBegin(GL_POINTS);
			{
				glVertex3dv(g_Vertex[0]);
				glVertex3dv(g_Vertex[3]);
				glVertex3dv(g_Vertex[4]);
				glVertex3dv(g_Vertex[5]);
			} glEnd();
		}
	}glDisable(GL_LINE_STIPPLE);

	//提取控制节点，绘制bezier曲线。
	//////////////////////////////////////////////////////////////////////////
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < CONTROL_POINT_NUM; i++) 
			glVertex3dv(&g_ControlVertex[i][0]);
	} glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, CONTROL_POINT_NUM, &g_ControlVertex[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glBegin(GL_LINE_STRIP);
	{
		for (int i = 0; i <= 30; i++) 
			glEvalCoord1f((GLfloat) i/30.0f);
	} glEnd();
	//////////////////////////////////////////////////////////////////////////
		

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
	//获取矩阵信息
	glGetIntegerv(GL_VIEWPORT, g_Viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, g_ModelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, g_ProjMatrix);

	if (button==GLUT_LEFT && state==GLUT_DOWN)
	{
		g_bIsDown = true;

		double _3dPoint[3];
		y = g_Viewport[3] - y;

		if (FIRST_LINE==g_iFlag)
		{
			gluUnProject( x, y, 0,
				g_ModelMatrix, g_ProjMatrix, g_Viewport,
				&_3dPoint[0], &_3dPoint[1], &_3dPoint[2] );

			// 全部初始化
			for (int i=0; i<3; i++)
				g_ControlVertex[0][i] = g_Vertex[0][i] = _3dPoint[i];
		}
		else if (SECOND_LINE==g_iFlag)
		{
			gluUnProject( x, y, 0,
				g_ModelMatrix, g_ProjMatrix, g_Viewport,
				&_3dPoint[0], &_3dPoint[1], &_3dPoint[2] );

			// 主要是保存v(3)
			for (int i=0; i<3; i++)
				g_ControlVertex[3][i] = g_Vertex[3][i] = _3dPoint[i];
		}
	}
		
	if (button==GLUT_LEFT && state==GLUT_UP)
	{
		g_bIsDown = false;
		
		g_iFlag = (g_iFlag+1) % 3;
		
		glutSetCursor( GLUT_CURSOR_RIGHT_ARROW );
	}
}

//////////////////////////////////////////////////////////////////////////
// 计算控制节点
void motion(int x, int y)
{
	if ( !g_bIsDown )
		return;
	
	glutSetCursor( GLUT_CURSOR_CROSSHAIR );
	y = g_Viewport[3] - y;

	if ( FIRST_LINE==g_iFlag )
	{
		gluUnProject( x, y, 0,
			g_ModelMatrix, g_ProjMatrix, g_Viewport,
			&g_Vertex[1][0], &g_Vertex[1][1], &g_Vertex[1][2] );

		for (int i=0; i<3; i++)
		{
			// 记录控制结点
			g_ControlVertex[1][i] = g_Vertex[1][i];

			// V0为V1和V2的中点
			g_Vertex[2][i] = 2 * g_Vertex[0][i] - g_Vertex[1][i];
		}
	}
	else if ( SECOND_LINE==g_iFlag )
	{
		gluUnProject( x, y, 0,
			g_ModelMatrix, g_ProjMatrix, g_Viewport,
			&g_Vertex[5][0], &g_Vertex[5][1], &g_Vertex[5][2] );

		for (int i=0; i<3; i++)
		{
			// V3为V4和V5的中点
			g_Vertex[4][i] = 2 * g_Vertex[3][i] - g_Vertex[5][i];

			// 记录控制结点
			g_ControlVertex[2][i] = g_Vertex[4][i];
		}
	}
		
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////
//  [9/27/2009 Bible]
//	组合类