#include <stdlib.h>
#include <GL/glut.h>

// User Constants

const GLfloat X = 0.525731112119133606;
const GLfloat Z = 0.850650808352039932;

GLfloat	g_spin = 0.0f;

GLfloat vertex_data[][3] = {
	{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
			{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
			{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
GLint vertex_idx[] = {
	0,4,1, 0,9,4, 9,5,4, 4,5,8, 4,8,1,
	8,10,1, 8,3,10,5,3,8, 5,2,3, 2,7,3,
	7,10,3, 7,6,10, 7,11,6, 11,0,6, 0,1,6,
	6,1,10, 9,0,11, 9,11,2, 9,2,5, 7,2,11
};

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void display (void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // Black Background
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear the colour and depth buffer
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef(0.0, 0.0, -10.0);

	//glutSolidCube(2.0f);
	glRotatef(g_spin,1.0,1.0,1.0);      // Rotate the Icosahedron so that we can visualise it properly 
	glColor3f(1,1,1);					// Generate a unique colour for each triangle 
	for (int i=0; i<20; i++)
	{
		//glColor3f(i/19,i/19,i/19);       // Generate a unique colour for each triangle 
		glBegin(GL_TRIANGLES);
			glVertex3fv( vertex_data[ vertex_idx[3*i] ] );
			glVertex3fv( vertex_data[ vertex_idx[3*i+1] ] );
			glVertex3fv( vertex_data[ vertex_idx[3*i+2] ] );
		glEnd();
	}             
	
	g_spin += 1.0f;
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (float)w/h, 1.0, 20.0);	

	//glMatrixMode (GL_MODELVIEW);
	//glLoadIdentity ();
	//glTranslatef(0.0, 0.0, -10.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{	
	case 27:
		exit(1);
		break;
	default:
		break;
	}
}


void mouse(int button, int state, int x, int y)
{
}

int main(int argc, char* argv[])
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	init();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop ();

	return 0;
}

//GLint vertex_idx[] = {
//			   (0,4,1), (0,9,4), (9,5,4), (4,5,8), (4,8,1),
//			   (8,10,1), (8,3,10),(5,3,8), (5,2,3), (2,7,3),
//			   (7,10,3), (7,6,10), (7,11,6), (11,0,6), (0,1,6),
//			   (6,1,10), (9,0,11), (9,11,2), (9,2,5), (7,2,11)
//};