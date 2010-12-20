#include <GL/glut.h>

//////////////////////////////////////////////////////////////////////////
//
float	g_fWidth = 1024;
float	g_fHeight= 768;
void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef (1.0, 2.0, 1.0);      /* modeling transformation */ 
	glutWireCube (1.0);

	/*
	*	The second viewport:位于屏幕的右上方.	
	*/
	glViewport ((GLsizei) g_fWidth/2, (GLsizei) g_fHeight/2, (GLsizei)g_fWidth, (GLsizei)g_fHeight);
	//glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluPerspective(60,(GLsizei)g_fWidth/(GLsizei)g_fHeight,1.5,20);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();          /* clear the matrix */
	/* viewing transformation  */
	glColor3f(1.0,0.0,1.0);
	gluLookAt (0, 0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glTranslated(-2.0, -2.0, 0.0);
	glutWireTeapot(1.0);

	/*
	*	The third viewport: draw each viewport's edges.	
	*/
	glViewport (0, 0, (GLsizei)g_fWidth, (GLsizei)g_fHeight);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//	gluOrtho2D(-w/2,-h/2,w/2,h/2);
	gluPerspective(60,(GLsizei)g_fWidth/(GLsizei)g_fHeight,1.5,20);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (0, 0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColor3f(1.0,1.0,1.0);
// 	glBegin(GL_LINES);
// 	glVertex2d( 0, g_fWidth/2);
// 	glVertex2d( g_fWidth, g_fHeight/2);				
// 	glVertex2d( g_fWidth/2, 0);				
// 	glVertex2d( g_fWidth/2, g_fHeight);
// 	glEnd();
	glScalef (1.0, 2.0, 1.0);      /* modeling transformation */ 
	glutWireCube (1.0);

	glFlush();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w/2, (GLsizei) h/2);
	glViewport ((GLsizei) w/2, (GLsizei) h/2, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluPerspective(60, (GLsizei)w/(GLsizei)h,1.5,20);
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  case 27:
		  exit(0);
		  break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (g_fWidth, g_fHeight); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
//			Y轴
//			|	
//			|
//			|
//			|____________X 轴
//		   /	
//		  /	
//		 /
//		Z轴
