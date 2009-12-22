#include "Helper.h"
#include <string>
#include <gl/freeglut.h>
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
bool	g_bIsShowInfo = false;

static char *FontList [] =
{
	"The Multiway  Hierarchy Tree Tutorial", 
	"r/R - rotate the tree",
	"v/V - in the pre-order visit the tree node",
	"p/P - in the post-order visit the tree node",
	"l/L - in the level-order visit the tree node",
	"Esc - Quit the demo",
	"Create Time - 4/10/2009",
	"Author - RenYaFei",
	"QQ - 546952720",
	"Email - dizuo@126.com"
};

void init();
void display();
void keyboard(unsigned char key, int x, int y);
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
	
	glutMainLoop();
	return 0;
}

void init(void) 
{
	glClearColor(84/255.0, 25.0/255.0, 125.0/255.0, 0.0);
	glShadeModel (GL_SMOOTH);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	{
		glColor3f (1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);
		glutSolidTeapot(2.0);
	}glPopMatrix();

	ENTER_ORTHO_PROJECTION()
	glPushMatrix();
	{
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLoadIdentity();
		for (size_t i(0); i<10; i++)
		{
			Helper::renderBitmapString2D(posX, posY+offset*i, (void *)font, FontList[i]); 
		}
	}glPopMatrix();
	LEAVE_PRTHO_PROJECTION()

	glutPostRedisplay();
	glutSwapBuffers();

	if ( !g_bRunFlag )
	{
		// cleanup system
		// ...
		// exit
		exit(0);
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
		case 'r':
		case'R':
			angle += 2.0;
			break;
	}
	glutPostRedisplay();
}

void setOrthographicProjection() {

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
	glLoadIdentity();
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
		glColor4f(1.0, 1.0, 1.0, 1.0);
		for (size_t i(0); i<Helper::getBufferSize(FontList); i++)
		{
			Helper::renderBitmapString2D(posX, posY+offset*i, (void *)font, FontList[i]); 
		}
	}glPopMatrix();
}