#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>

#define TIMER 33

static float xrot;

#define TOGGLE_ROTATE 1
#define STEP_ROTATE 2
static int rotate, step;

static int ext10Available = 0;


GLUquadric *quad;


void drawObj ()
{
	glPushMatrix ();
	glTranslatef (0., 0., 2.);
	gluDisk (quad, 0., .5, 18, 1);
	glPopMatrix ();

	gluCylinder (quad, .5, .5, 2., 18, 8);

	glPushMatrix ();
	glScalef (1., 1., -1.);
	gluDisk (quad, 0., .5, 18, 1);
	glPopMatrix ();
}


static void display( void )
{
	int i;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
	gluLookAt (0., 0., 5.,
		0., 0., 0.,
		0., 1., 0.);

	glRotatef (xrot, 1., 0., 0.);
	glTranslatef (0., 0., -1.);

	for (i=0; i<2; i++) {
		glPushMatrix ();
		glTranslatef ((i?.6:-.6), 0., 0.);

		glColor3f (1., 1., 1.);
		drawObj();

		glPushAttrib (GL_ALL_ATTRIB_BITS);
		if (i==0) {
			if (ext10Available) {
				/* Use OpenGL 1.0 polygon offset extension */
#ifdef GL_EXT_polygon_offset
				glEnable (GL_POLYGON_OFFSET_EXT);
				glPolygonOffsetEXT (1., 1./(float)0x10000);
#endif /* GL_EXT_polygon_offset */
			}
		}
		else {
			/* Use OpenGL 1.1 polygon offset feature */
#ifdef GL_VERSION_1_1
			glEnable (GL_POLYGON_OFFSET_FILL);
			glPolygonOffset (1., 1.);
#endif /* GL_VERSION_1_1 */
		}
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glEnable (GL_LIGHTING);
		glColor3f (.2, .2, 1.);
		drawObj();
		glPopAttrib ();

		glPopMatrix ();
	}

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport (0, 0, w, h);       
	glMatrixMode (GL_PROJECTION);  
	glLoadIdentity ();
	gluPerspective (50., (float)w/(float)h, 1., 20.);
}

static void cbMainMenu (int value)
{
	if (value == 99) {
		exit (0);
	}
	else if (value == TOGGLE_ROTATE) {
		rotate = !rotate;
	}
	else if (value == STEP_ROTATE) {
		step = 1;
	}
}


static void timer (int value)
{
	if (rotate || step) {
		step = 0;
		xrot += 1.f;
		if (xrot > 360.f) xrot -= 360.f;

		glutPostRedisplay ();
	}

	glutTimerFunc (TIMER, timer, 0);
}


static void init ()
{
	int mainMenu;
	const GLubyte *str;

	str = glGetString (GL_EXTENSIONS);
	ext10Available = (strstr((const char *)str, "GL_EXT_polygon_offset") != NULL);

	xrot = 0.;

	glDisable (GL_DITHER);
	glEnable (GL_DEPTH_TEST);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);


	{
		GLfloat pos[4] = {3., 5., 2., 1.};
		GLfloat white[4] = {1., 1., 1., 1.};
		GLfloat black[4] = {0., 0., 0., 0.};

		/* Set up light1 */
		glDisable (GL_LIGHTING);
		glEnable (GL_LIGHT1);
		glLightfv (GL_LIGHT1, GL_POSITION, pos);
		glLightfv (GL_LIGHT1, GL_DIFFUSE, white);
		glLightfv (GL_LIGHT1, GL_SPECULAR, black);

		/* ambient and diffuse will track glColor */
		glEnable (GL_COLOR_MATERIAL);
		glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv (GL_FRONT, GL_SPECULAR, black);
	}

	quad = gluNewQuadric ();

	glutDisplayFunc (display); 
	glutReshapeFunc (reshape);
	glutTimerFunc (TIMER, timer, 0);

	mainMenu = glutCreateMenu (cbMainMenu);
	glutAddMenuEntry ("Toggle Rotate", TOGGLE_ROTATE);
	glutAddMenuEntry ("Step Rotate", STEP_ROTATE);
	glutAddMenuEntry ("Quit", 99);
	glutAttachMenu (GLUT_RIGHT_BUTTON);
	rotate = 1;
	step = 0;
}

void main(int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); 
	glutInitWindowSize (300,300);
	glutInitWindowPosition (0,0); 
	glutCreateWindow ("Polygon Offset: 1.0 versus 1.1");

	init ();

	glutMainLoop ();
}


