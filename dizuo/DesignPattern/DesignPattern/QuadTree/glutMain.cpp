#include <ctime>
#include <string>
#include "Helper.h"
#include "QuadTree.h"
#include "Vec2.hpp"

using namespace std;

#define ENTER_ORTHO_PROJECTION() setOrthographicProjection();
#define LEAVE_PRTHO_PROJECTION() resetPerspectiveProjection();

float	angle = 0;
float	windowWidth = 1024;
float	windowHeight = 768;
int		font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
int		posX = 20;
int		posY = 50;
int		offset = 30;
bool	g_bRunFlag = true;
bool	g_bIsShowInfo = true;		

QuadTree * g_pQTree = NULL;

Vec2f	g_MousePos;
Vec2f	g_MouseMove;
bool	g_bIsDrag = false;
bool	g_bIsSelect = false;	//右键选择content区域
QuadTreeNode::rectContainer g_queryContainer;

static char *FontList [] =
{
	"The Quad Tree Tutorial", 
	"Esc, Q, q - Quit the demo",
	"LMouse Button - Drag the elipse and insert it into the quadTree's contents",
	"RMouse Button - Select the contents",
	"  When the elipse content are selected, their borders are black",
	"H, h - Toggle the helper board visibility",
	"Create Time - 4/22/2009",
	"Author - RenYaFei",
	"QQ - 546952720",
	"Email - dizuo@126.com"
};

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void reshape(int w, int h);

void setOrthographicProjection();	//正交投影绘制font
void resetPerspectiveProjection();	//绘制场景使用透视投影

void showAuthorInfo();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (windowWidth, windowHeight); 
	glutInitWindowPosition (100, 100);
	int window = glutCreateWindow (argv[0]);
	init ();
	
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	while ( g_bRunFlag )
	{
		glutMainLoopEvent();
		display();
	}
	glutDestroyWindow(window);
	delete g_pQTree;
	g_pQTree = NULL;

	return 0;
}

void init(void) 
{
	glClearColor(84/255.0, 25.0/255.0, 125.0/255.0, 0.0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_FLAT);

	//初始化
	g_bRunFlag = true;
	//t b l r
	g_pQTree = new QuadTree( TRectanglef(0.0, windowHeight, 0.0, windowWidth) );
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glLoadIdentity ();             /* clear the matrix */
	//只用于三维场景绘制 对于正交投影没有影响
	gluLookAt (20.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	{
		glRotatef(angle, 0.0, 1.0, 0.0);
		//render 3D scene:

	}glPopMatrix();

	ENTER_ORTHO_PROJECTION()
	glLoadIdentity();		//必不可少
	glPushMatrix();
	{
		glColor4f(1.0, 0.0, 0.0, 1.0);
		//g_pQTree->insertArea( TRectanglef(Vec2f(10, 10), Vec2f(20, 20)) );
		g_pQTree->recursRenderNode( g_pQTree->getRoot() );
	}glPopMatrix();
	
	if( g_bIsDrag )		//绘制鼠标左键拖出的椭圆
		Helper::renderElipse( TRectanglef( g_MousePos, g_MouseMove ), Color4(0x556B2F), false );
	
	if( g_bIsSelect )	//绘制用来选择的白框
	{
		Helper::renderRectangle( TRectanglef(g_MousePos, g_MouseMove), Color4(0xFFFFFF) );
	}

 	for (size_t i(0); i<g_queryContainer.size(); i++)
 	{
 		Helper::renderElipse( g_queryContainer[i], Color4( 0x000000 ) );	
 	}

	if(g_bIsShowInfo)
		showAuthorInfo();

	LEAVE_PRTHO_PROJECTION()

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	gluPerspective(80, (float)(windowWidth)/(float)(windowHeight), 1.5, 500);
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
		case 'q':
		case 'Q':
			g_bRunFlag = false;
			break;
		case 'h':
		case 'H':
			g_bIsShowInfo = !g_bIsShowInfo;
			break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT && state == GLUT_DOWN )
	{
		g_MousePos.setValue( x, y );
		g_bIsDrag = true;

		//退出查询模式：
		g_queryContainer.clear();
	}

	if( button == GLUT_LEFT && state == GLUT_UP )
	{
		int sizex = x - g_MousePos.x();
		int sizey = y - g_MousePos.y(); 
		g_pQTree->insertArea( TRectanglef( g_MousePos, Vec2f(sizex, sizey) ) );

		//清理上次的记忆
		g_bIsDrag = false;
		g_MouseMove.setValue(0, 0);
	}

	if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
	{
		g_bIsSelect = true;
		g_bIsDrag = false;
		g_MousePos.setValue( x, y );

		g_queryContainer.clear();
	}

	if( button == GLUT_RIGHT_BUTTON && state == GLUT_UP )
	{
		g_bIsSelect = false;
		g_MouseMove.setValue(0, 0);

		//Query the selected area
		int sizex = x - g_MousePos.x();
		int sizey = y - g_MousePos.y(); 
		g_queryContainer = g_pQTree->queryArea( TRectanglef( g_MousePos, Vec2f(sizex, sizey) ) );
	}
}

void motion(int x, int y)
{
	if( g_bIsDrag || g_bIsSelect )
		g_MouseMove = Vec2f(x, y) - g_MousePos;

	glutPostRedisplay();
}

void setOrthographicProjection() 
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -windowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() 
{
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void showAuthorInfo() 
{
 	glPushMatrix();
	{
		int num = Helper::getBufferSize(FontList) - 1;
		//display the panel
		glColor4f(1.0, 0.0, 0.0, 1.0);
		Helper::renderRectangle( TRectanglef(Vec2f(posX, posY-30.0), Vec2f(800.0, posY+offset*num)) );
		Helper::renderElipse( TRectanglef(Vec2f(posX, posY-30.0), Vec2f(800.0, posY+offset*num)),
							  Color4(0x556B2F), false );
 		glColor4f(1.0, 1.0, 1.0, 1.0);
 		for (size_t i(0); i<Helper::getBufferSize(FontList); i++)
 		{
 			Helper::renderBitmapString2D(posX, posY+offset*i, (void *)font, FontList[i]); 
 		}
 	}glPopMatrix();
}