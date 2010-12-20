/*
*  torus.c
*  This program demonstrates the creation of a display list.
*/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI_ 3.14159265358979323846

GLuint theTorus;

/* Draw a torus */
// numc 为XOY平面上的分段数
// numt 为XOZ平面上的分段数
// 内半径：1 - 0.3
// 外半径：1 + 0.3
static void torus(int numc, int numt)
{
	int i, j, k;
	double s, t, x, y, z, twopi;

	twopi = 2 * PI_;
	for (i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= numt; j++) {
			//多绘制一段首尾相接
			for (k = 1; k >= 0; k--) {
				s = (i + k) % numc + 0.5;
				t = j % numt;

				x = (1+.3*cos(s*twopi/numc))*cos(t*twopi/numt);
				y = (1+.3*cos(s*twopi/numc))*sin(t*twopi/numt);
				z = .3 * sin(s * twopi / numc);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
}

//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
//  Create a torus be giving inner, outer radius and detail level   //
//------------------------------------------------------------------//
void DrawTorus(float TubeRadius, float Radius, int Sides, int Rings)
{
	int I, j;
	float theta, phi, theta1;
	float cosTheta, sinTheta;
	float cosTheta1, sinTheta1;
	float ringDelta, sideDelta;
	float cosPhi, sinPhi, dist;

	// Theta控制XOY平面
	// Phi	控制XOZ平面
	sideDelta = 2.0f * PI_ / Sides;
	ringDelta = 2.0f * PI_ / Rings;
	theta = 0.0f;
	cosTheta = 1.0f;
	sinTheta = 0.0f;

	for (int i=Rings-1; i>=0; i--)
	//for (int i=0; i>=0; i--)
	{
		theta1 = theta + ringDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);
		glBegin(GL_QUAD_STRIP);
		{
			phi = 0.0f;
			for (j = Sides; j>=0; j--)
			{
				phi = phi + sideDelta;
				cosPhi = cos(phi);
				sinPhi = sin(phi);
				dist = Radius + (TubeRadius * cosPhi);

				glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
				glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, TubeRadius * sinPhi);

				glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
				glVertex3f(cosTheta * dist, -sinTheta * dist, TubeRadius * sinPhi);
			}
		}glEnd();
		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}
//////////////////////////////////////////////////////////////////////////

/* Create display list with Torus and initialize state */
static void init(void)
{
// 	theTorus = glGenLists (1);
// 	glNewList(theTorus, GL_COMPILE);
// 	torus(8, 25);
// 	glEndList();

	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

/* Clear window and draw torus */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, .0);
	//glCallList(theTorus);
	 DrawTorus( 0.6, 1.8, 10, 20);    
	glFlush();
}

/* Handle window resize */
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

/* Rotate about x-axis when "x" typed; rotate about y-axis
when "y" typed; "i" returns torus to original view */
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
   case 'x':
   case 'X':
	   glRotatef(30.,1.0,0.0,0.0);
	   glutPostRedisplay();
	   break;
   case 'y':
   case 'Y':
	   glRotatef(30.,0.0,1.0,0.0);
	   glutPostRedisplay();
	   break;
   case 'i':
   case 'I':
	   glLoadIdentity();
	   gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	   glutPostRedisplay();
	   break;
   case 27:
	   exit(0);
	   break;
	}
}

int main(int argc, char **argv)
{
	glutInitWindowSize(200, 200);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

// Texture Mapping a Torus
// We can define a torus by the equations:
// [Note: p = Pi = 3.141592. Define q as the angle from the X axis (0 <= q <= 2p) and f as the angle from the Z axis (0.0 <= f <= p).]
// 
// Then, with R as the inner radius and r as the outer radius:
// X = (R + r cos (2 pv)) cos (2 pu)
// Y = r sin (2 pv) 
// Z = (R + r cos (2 pv)) sin (2 pu)
// 
// Then: 
// v = arccos (Y/R)/2p
// u = [arccos ((X/(R + r*cos(2 pv))]2p
