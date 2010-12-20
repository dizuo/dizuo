/*
 *  blendeqn.c
 *  Demonstrate the different blending functions available with the
 *  OpenGL imaging subset.  This program demonstrates use of the
 *  glBlendEquation() call.
 *
 *  The following keys change the selected blend equation function:
 *
 *      'a'  ->  GL_FUNC_ADD
 *      's'  ->  GL_FUNC_SUBTRACT
 *      'r'  ->  GL_FUNC_REVERSE_SUBTRACT
 *      'm'  ->  GL_MIN
 *      'x'  ->  GL_MAX
 */

#include <gl/glew.h>
#include <GL/glut.h>
//#include <stdlib.h>
//#include <gl/glxext.h>

#pragma comment (lib,"glew32.lib")
#pragma comment (lib, "glew32s.lib")

// #define GL_MIN					0x8007
// #define GL_MAX					0x8008
// #define GL_FUNC_ADD				0x8006
// #define GL_FUNC_SUBTRACT			0x800A
// #define GL_FUNC_REVERSE_SUBTRACT		0x800B


void init(void)
{
   glClearColor(1.0, 1.0, 0.0, 0.0);

   glBlendFunc(GL_ONE, GL_ONE);
   glEnable(GL_BLEND);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 1.0);
   glRectf(-0.5,-0.5,0.5,0.5);

   glFlush();
}

void reshape(int w, int h)
{
   GLdouble aspect = (GLdouble) w / h;

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (aspect < 1.0) {
     aspect = 1.0 / aspect;
     glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
   } else
     glOrtho(-1.0, 1.0, -aspect, aspect, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'a': case 'A':
	 /* Colors are added as: (1, 1, 0) + (0, 0, 1) = (1, 1, 1)
	  *  which will produce a white square on a yellow background.
	  */
	 glBlendEquation(GL_FUNC_ADD);
	 break;

      case 's': case 'S':
	 /* Colors are subtracted as: (0, 0, 1) - (1, 1, 0) = (-1, -1, 1)
	  *  which is clamped to (0, 0, 1), producing a blue square on a
	  *  yellow background
	  */	 glBlendEquation(GL_FUNC_SUBTRACT);
	 break;

      case 'r': case 'R':
	 /* Colors are subtracted as: (1, 1, 0) - (0, 0, 1) = (1, 1, -1)
	  *  which is clamed to (1, 1, 0).  This produces yellow for both
	  *  the square and the background.
	  */
	 glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	 break;

      case 'm': case 'M':
	 /* The minimum of each component is computed, as
	  *  [min(1, 0), min(1, 0), min(0, 1)] which equates to (0, 0, 0).
	  *  This will produce a black square on the yellow background.
	  */
	 glBlendEquation(GL_MIN);
	 break;

      case 'x': case 'X':
	 /* The minimum of each component is computed, as
	  *  [max(1, 0), max(1, 0), max(0, 1)] which equates to (1, 1, 1)
	  *  This will produce a white square on the yellow background.
	  */
	 glBlendEquation(GL_MAX);
	 break;

      case 27:
         exit(0);
   }
   
   glutPostRedisplay();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
	//GLenum err = glewInit() ;
	//if (GLEW_OK != err)
	//{    ///加入你自己的错误处理
	//	//printf("init failed..\n");
	//	exit(0);
	//}

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(512,512);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
