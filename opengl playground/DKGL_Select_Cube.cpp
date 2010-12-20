#include <cstdlib>
#include <gl/glut.h>

#include "dkgl/dkglViewport.h"
using namespace DKGL;

const int		BUFSIZE = 512;
int				g_iWidth = 800;
int				g_iHeight = 600;
float			g_fAngle = 0;
const GLfloat	red[] = { 231/255.0, 36.0/255.0, 46/255.0,1.0};
const GLfloat	yellow[] = { 253.0/255.0, 208.0/255.0, 0.0/255., 1.0};

class Cube
{
public:
	double x,y,z;
	bool selected;

	GLdouble screenPos[3];
	
	Cube()
		: x(0), y(0), z(0)
		, selected(false)
	{
		for (size_t i(0); i<3; i++)
		{
			screenPos[i] = 0;
		}
	}

	void Draw()
	{
		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];
		GLint viewport[4];

		glPushMatrix();
		{
			glTranslated(x, y, z);

			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
			gluProject( 0, 0, 0, modelMatrix, projMatrix, viewport,
						&screenPos[0], &screenPos[1], &screenPos[2] );

			if (selected) 
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
			else 
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);

			glutSolidCube(1.0);		//  对应到屏幕size:60
		}glPopMatrix();
	}
};
Cube	cube[4];
int		g_iViewNum = 4;	
ViewportManager* pViewMgr = new ViewportManager(g_iViewNum);

void Init();
void DrawGLScene();
void Reshape(int w, int h);
void KeyBoardCB(unsigned char key, int x, int y);
void MouseCB(int button, int state, int x, int y);

void Idle();
void PaintScreen(GLenum mode);
void RecursPaintScreen(GLenum mode, int index);

int main(int argc, char **argv) 
{  
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("OpenGL Select Obj");

	Init();
	glutDisplayFunc ( DrawGLScene );
	glutReshapeFunc ( Reshape );

	glutMouseFunc( MouseCB );
	glutKeyboardFunc( KeyBoardCB );
//	glutIdleFunc( Idle );

	glutMainLoop ();

	return 0;
}

void Init()	   
{
	glClearColor(84/255.0, 0.0/255.0, 125.0/255.0, 0.0);
	glClearDepth(1.0);				
	glDepthFunc(GL_LESS);			
	glEnable(GL_DEPTH_TEST);		
	glShadeModel(GL_SMOOTH);		

	for(int i=0; i<4; i++) 
		cube[i].x = i*2-3;
}

//////////////////////////////////////////////////////////////////////////
//0		1
//2		3
//////////////////////////////////////////////////////////////////////////
void Reshape(int Width, int Height)
{
	int halfW = Width / 2;
	int halfH = Height / 2;

	for (int i=0; i<g_iViewNum; i++)
	{
		int xIdx = i % 2;
		int yIdx = i / 2;

		int x = halfW * xIdx;
		int y = halfH * yIdx;

		pViewMgr->GetSingletonPtr()->resizeViewport(x, y, halfW, halfH, i);
	} pViewMgr->GetSingletonPtr()->updateClientRectSize(Width, Height);

	g_iWidth = Width;
	g_iHeight = Height;
}

void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, g_iWidth, g_iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				
	gluPerspective(45.0, (GLfloat)g_iWidth/(GLfloat)g_iHeight, 1.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(6.0,4.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//PaintScreen(GL_RENDER);
	RecursPaintScreen(GL_RENDER, 0);


	//////////////////////////////////////////////////////////////////////////
	// Last Render the viewport
	for (int i=0; i<g_iViewNum; i++)
		pViewMgr->GetSingletonPtr()->getViewport(i).drawBoarder();

 	glDisable(GL_LIGHT0);
 	glDisable(GL_LIGHTING);
 	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void PaintScreen(GLenum mode)
{
	glPushMatrix();
	glRotated( g_fAngle, 0.0, 1.0, 0.0 );
	for(int i=0; i<4; i++)
	{
		if (mode==GL_SELECT)
			glLoadName(i);
		cube[i].Draw();
	}
	glPopMatrix();
}

void RecursPaintScreen(GLenum mode, int index)
{
	if(index >= 4)
		return;
	glPushMatrix();
	glRotated( g_fAngle, 0.0, 1.0, 0.0 );
	{
		if (mode==GL_SELECT)
			glLoadName(index);
		cube[index].Draw();
	}
	glPopMatrix();

	RecursPaintScreen(mode, index+1);
}

void Idle()
{
	g_fAngle += 0.5;
	if( g_fAngle >= 360 )
		g_fAngle = 0;

	//调用DrawGLScene函数
	glutPostRedisplay();
}

void KeyBoardCB(unsigned char key, int x, int y)
{
	if (key=='w' || key=='W')
	{
		pViewMgr->GetSingletonPtr()->maximize(0);
	}
	if(key == 27)
		exit(0);

	glutPostRedisplay();
}

void MouseCB(int button, int state, int x, int y)
{
	if( button != GLUT_LEFT || state != GLUT_DOWN)
		return;

	int area = 50;
	for (size_t i(0); i<4; i++)
	{
		if( x>cube[i].screenPos[0]-area &&
			x<cube[i].screenPos[0]+area &&
			y>cube[i].screenPos[1]-area &&
			y<cube[i].screenPos[1]+area )
			cube[i].selected = !cube[i].selected;			
	}
	
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<g_iViewNum; i++)
	{
		pViewMgr->GetSingletonPtr()->activeViewport(i);
		pViewMgr->GetSingletonPtr()->selectViewport(x, y);
	}
	//////////////////////////////////////////////////////////////////////////

	glutPostRedisplay();
}