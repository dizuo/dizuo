#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#define DATA 5000 // maximum data
int ndata;
float x[DATA], y[DATA], oldy[DATA];
#define OFFSET 1
#define hh(n)   h[n+OFFSET]
// popup menu for Mallat order or Lift order
//
double dx;

void cascade () 
{
    double h[7];
    int i, n, k, h_len, hmin, hmax;
    
	// filter definition D4
	h_len = 4;    
/*
	h[0] = (1+sqrt(3.0))/4;  // H = 2h(n)
    h[1] = (3+sqrt(3.0))/4; 
    h[2] = (3-sqrt(3.0))/4; 
    h[3] = (1-sqrt(3.0))/4;

	h[0]=-1./16;
	h[1]=0;
	h[2]=9./16;
	h[3]=1.0;
	h[4]=9./16;
	h[5]=0;
	h[6]=-1./16;
	hmin = -3, hmax = 3; // offset = 3
*/
/* this portion only applies to boundary and cannot be uniformly
   applied after first iteration ...
*/
	h[0]=15./16;
	h[1]=1.0;
	h[2] = 9./16;
	h[3]=0.0;
	h[4]=-1./16;
	hmin=-1, hmax=3;  // offset = 1


    // copy old data
    for (i = 0; i < ndata; i++) 
 		oldy[i] = y[i];

    // upsampling by two
    ndata *= 2;
    dx /= 2;
    for (i = 0; i < ndata; i++) {
    	x[i] = i*dx;
    	if (i%2)  // i is odd; insert zero
    	   y[i] = 0;
    	else
    	   y[i] = oldy[i/2];
    }
//	for (i = 0; i < ndata; i++) 
//		printf ("%lf\n", y[i]);
//getchar();

	// convolve with H
    for (i = 0; i < ndata; i++) 
 		oldy[i] = y[i];

	for (n = 0; n < ndata+(-hmin-1); n++) {
	    y[n] = 0;
		for (k = hmin; k <= hmax; k++) {
			y[n] += hh(k)*oldy[n-k];
		}
	}
	for (i = 0; i < ndata; i++) 
		printf ("%lf\n", y[i]);
	// do not plot outside ndata!
}
    


void SetData()
{
    int i, which;
    double sum_y, sum_yy;

	// ndata larger indicates finer level scaling fun/wavelets
	// value gets larger (due to the required integral property)
	// mother wavelet corresponds to ndata = ?!
	//
	// this line of thoughts is not quite right. ndata represents
	// the dimension of the space. Any space could be regarded as
	// V0 and the scaling function be the corresponding mother fun.
	// different "which" collectively represent the basis
	// of that space.
    ndata = 8;
    which = 1;    // must be in [0, ndata-1]
    
	for (i = 0; i < ndata; i++) {
       y[i] = 0;
       dx = 1.0;   // dx = 1 is the fact for V0
	               // what about the basis for V1?
	               // one should convolve the V0 basis with H again
       x[i] = i*dx;
    }
    y[which] = 1.0;


   for (i = 0; i < 1; i++) {
      cascade(); 
   }

   // print out integral, energy value
   sum_y = sum_yy = 0;
   for (i = 0; i < ndata; i++) {
	   sum_y += y[i];
	   sum_yy += y[i]*y[i];
   }
   printf ("%lf %lf\n", sum_y*dx, sum_yy*dx);
}

int drawmode=2;

void draw_mode (int value) 
{ 	
   switch (value) {
     case 1:
	   drawmode = GL_POINTS;
	   glPointSize (2.0);
     break;
     case 2:
	   drawmode = GL_LINE_STRIP;
     break;
   }
   glutPostRedisplay();
}

float max[2], min[2];
void FindBound ()
{
   int i;
   
   max[0] = max[1] = -1e5;
   min[0] = min[1] = 1e5;
   // raw bound
   for (i = 0; i < ndata; i++) {
   	if (x[i] > max[0]) max[0] = x[i];
   	if (y[i] > max[1]) max[1] = y[i];
   	if (x[i] < min[0]) min[0] = x[i];
   	if (y[i] < min[1]) min[1] = y[i];
   }
}

void DrawAxes ()
{
#define drawOneLine(x1,y1,x2,y2)  glBegin(GL_LINES);  \
   glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();
#define Min(x,y) ((x) < (y) ? (x) : (y))

    drawOneLine (min[0], 0.0, max[0], 0.0);    // x-axis
    drawOneLine (min[0], Min(0,min[1]), min[0], max[1]);  // y-axis
}
  

void init(void) 
{
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glLineWidth (2.0);
   FindBound ();
   glShadeModel (GL_FLAT);
}

void display(void)
{
   int i;
	glClear (GL_COLOR_BUFFER_BIT);

   glColor3f (0.0, 0.0, 0.0);
   DrawAxes();

// draw grid
// draw scale

   glColor3f (1.0, 0.0, 0.0);
   glBegin (drawmode);
     glVertex2f (x[0], y[0]);
     for (i = 1; i < ndata; i++) 
     	glVertex2f (x[i], y[i]);
   	 glEnd();
   glFlush ();
}

void reshape (int w, int h)
{
   float left, right, top, bottom;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   left = (min[0]+max[0])/2 - (max[0]-min[0])/2*1.5;
   right = (min[0]+max[0])/2 + (max[0]-min[0])/2*1.5;
   bottom = (min[1]+max[1])/2 - (right-left)/2;
   top = (min[1]+max[1])/2 + (right-left)/2;
   gluOrtho2D (left, right, bottom, top);
}

void 
main_menu(int value)
{
    if (value == 666)
        exit(0);
	if (value == 777) {
		cascade();
        glutPostRedisplay();
	}
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
   int submenu;
   SetData();
   
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (400, 400); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
       
   submenu = glutCreateMenu(draw_mode);
   glutAddMenuEntry("Points", 1);
   glutAddMenuEntry("Lines", 2);
    
   glutCreateMenu(main_menu);
   glutAddMenuEntry("Quit", 666);
   glutAddMenuEntry("Subdivide again", 777);
   glutAddSubMenu("Draw mode", submenu);
    
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   glutMainLoop();
   return 0;
}

