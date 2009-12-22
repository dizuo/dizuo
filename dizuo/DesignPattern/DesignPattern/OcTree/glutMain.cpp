#include <ctime>
#include <string>
#include "Helper.h"
#include "OcTree.h"

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

OcTree*	g_pOcTree = NULL;

static char *FontList [] =
{
	"The OcTree Tutorial", 
	"Esc, Q, q - Quit the demo",
	"R, r - Rotate the OcTree scene",
	"H, h - Toggle the helper board visibility",
	"Create Time - 4/20/2009",
	"Author - RenYaFei",
	"QQ - 546952720",
	"Email - dizuo@126.com"
};

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
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
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc( mouse );

	glutMainLoop();
	
	return 0;
}

void init(void) 
{
	//glClearColor(84/255.0, 25.0/255.0, 125.0/255.0, 0.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	//初始化
	g_bRunFlag = true;
	//t b l r
	srand( (unsigned)time( NULL ) );
	float size = 4.0;
	g_pOcTree = new OcTree( Box3f( Vec3f(-size, -size, -size), Vec3f(size, size, size) ) );
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glLoadIdentity ();            
	//只用于三维场景绘制 对于正交投影没有影响
	gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	{
		glRotatef(angle, 0.0, 1.0, 0.0);
		//render 3D scene:
		//ColorCube::renderColorCube( Box3f( Vec3f(-2.0, -2.0, -2.0), Vec3f(2.0, 2.0, 2.0) ) );		//绘制彩色的cube/
		g_pOcTree->insertArea( Box3f(Vec3f(0.5, 0.5, 0.5), Vec3f(1.0, 1.0, 1.0)) );
		g_pOcTree->recursRenderNode( g_pOcTree->getRoot() );
	}glPopMatrix();

	ENTER_ORTHO_PROJECTION()
	glPushMatrix();
	{
		glLoadIdentity();		//必不可少
		glColor4f(1.0, 0.0, 0.0, 1.0);
	}glPopMatrix();
	
	if(g_bIsShowInfo)
		showAuthorInfo();

	LEAVE_PRTHO_PROJECTION()

	glutPostRedisplay();
	glutSwapBuffers();

	if( !g_bRunFlag )
	{
		//clean up the system
		//...
		delete g_pOcTree;
		g_pOcTree = NULL;
		exit(1);
	}
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
		case 'r':
		case 'R':
			angle += 2.0;
			break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT && state == GLUT_DOWN )
	{
	}

	if( button == GLUT_LEFT && state == GLUT_UP )
	{

	}
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
		glLoadIdentity();
		int num = Helper::getBufferSize(FontList) - 1;
		//display the panel
		glColor4f(1.0, 0.0, 0.0, 1.0);
		Helper::renderRectangle( TRectanglef(Vec2f(posX, posY-30.0), Vec2f(500.0, posY+offset*num)) );
		Helper::renderElipse( TRectanglef(Vec2f(posX, posY-30.0), Vec2f(500.0, posY+offset*num)),
							  Color4(0x556B2F), false );

 		glColor4f(1.0, 1.0, 1.0, 1.0);
 		for (size_t i(0); i<Helper::getBufferSize(FontList); i++)
 		{
 			Helper::renderBitmapString2D(posX, posY+offset*i, (void *)font, FontList[i]); 
 		}
 	}glPopMatrix();
}