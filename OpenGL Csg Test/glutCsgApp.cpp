#include <vector>
#include <GL/glut.h>
#include <cmath>

#include "./core/CSGWorld.h"
#include "./core/BooleanModeller.h"

using namespace std;

GLfloat g_fView[3] = {0.0, 0.0, 2.0};
GLfloat	g_fAngle[3] = {0.0};
int		g_iAreaSizeX = 64;
int		g_iAreaSizeY = 64;
const float g_fOffset = .5;

CSGWorld			g_csgTester;
BooleanModeller*	g_pBooleanModeller(0);
Solid*				g_pUnionModel(0);
Solid*				g_pDifferenceModel(0);
Solid*				g_pIntersectionModel(0);

void init();
void display();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);

void setup();
void destroy();
void viewTransform();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (800, 800); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}

void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	
	setup();
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (g_fView[0], g_fView[1], g_fView[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// normal
	glPushMatrix();
	{
		glTranslatef(-g_fOffset, g_fOffset, 0.0f);
			viewTransform();
		g_csgTester.Render();
	}glPopMatrix();

	// union
	glPushMatrix();
	{
		glTranslatef(g_fOffset, g_fOffset, 0.0f);
			viewTransform();
		g_pUnionModel->Render();
	}glPopMatrix();

	// difference
	glPushMatrix();
	{
		glTranslatef(-g_fOffset, -g_fOffset, 0.0f);
			viewTransform();
		g_pDifferenceModel->Render();
	}glPopMatrix();

	// intersection
	glPushMatrix();
	{
		glTranslatef(g_fOffset, -g_fOffset, 0.0f);
			viewTransform();
		g_pIntersectionModel->Render();
	}glPopMatrix();


	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluPerspective(60,1.0,0.0001,500);
	//gluOrtho2D( 0, w, 0, h );
	glMatrixMode (GL_MODELVIEW);
}

void viewTransform()
{
	glRotatef(g_fAngle[0], 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[1], 1.0, .0, 0.0);
	glRotatef(g_fAngle[2], 0.0, .0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 27:
		destroy();
		exit(0);
		break;
	case 'w':
		g_fAngle[0] += 5.0;
		break;
	case 's':
		g_fAngle[0] -= 5.0;
		break;
	case 'q':
		g_fAngle[1] += 5.0;
		break;
	case 'e':
		g_fAngle[1] -= 5.0;
		break;
	case 'x':
		g_fAngle[2] += 5.0;
		break;
	case 'd':
		g_fAngle[2] -= 5.0;
		break;
	
	// adjust the view point
	case 'n':
		g_fView[2] += 0.1f;
		break;
	case 'm':
		g_fView[2] -= 0.1f;
		break;

	// test for the boolean operation: 
	case '1':
		g_fAngle[0] = g_fAngle[1] = g_fAngle[2] = 0.0f;
		break;
	//case '2':
	//	g_csgTester.Difference(0, 1);
	//	break;
	//case '3':
	//	g_csgTester.Intersection(0, 1);
	//	break;
	}
	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}

void setup()
{
	g_csgTester.LoadCube();
	g_csgTester.LoadCone();
	//g_csgTester.LoadSphere();
	//g_csgTester.LoadCylinder();

	if (!g_pBooleanModeller)
		g_pBooleanModeller = 
		new BooleanModeller( g_csgTester.GetSolid(0),
			g_csgTester.GetSolid(1) );

	if (!g_pUnionModel)
		g_pUnionModel = g_pBooleanModeller->getUnion();

	if (!g_pDifferenceModel)
		g_pDifferenceModel = g_pBooleanModeller->getDifference();

	if (!g_pIntersectionModel)
		g_pIntersectionModel = g_pBooleanModeller->getIntersection();
}

void destroy()
{
	if (g_pBooleanModeller)
		delete g_pBooleanModeller;

	if (g_pUnionModel)
		delete g_pUnionModel;

	if (g_pDifferenceModel)
		delete g_pDifferenceModel;

	if (g_pIntersectionModel)
		delete g_pIntersectionModel;
}