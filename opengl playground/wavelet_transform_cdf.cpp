//
// Example to Assignment two 
// Introduction to Wavelet, Spring 2001
// 
// Remark: only implement linear interpolating wavelet transform CDF (2,2)
// Intent: demonstrate data access in lifting order
//
// copyright: Jyun-Ming Chen
//
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

#define DATA 500 // maximum data
int ndata;
float x[DATA], y[DATA];  // for plotting
float ss[DATA];  // for storing signal
float sold[DATA]; // copy of untransformed data
// (for restoring data only)

//
// lifting indexing
//
#define JMAX  5   // assume sample size 2^JMAX
int INCR[10]={1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

//
// macros for accessing the data array
//
#define S(j,l) ss[(l)*INCR[JMAX-(j)]]                     // increment
#define D(j,l) ss[INCR[JMAX-((j)+1)]+(l)*INCR[JMAX-(j)]]  // offset + increment


//
// signal initialization
//
void InitSignal()
{
	int i;

	ndata = powf(2,JMAX);

	// ex: simple sine wave
	for (i = 0; i < ndata; i++)
		ss[i] = sinf(i);

	// make a copy (for later restoration
	for (i = 0; i < ndata; i++)		
		sold[i] = ss[i];
}

// functions involving the macros for data access
//
// (classical wavelet): assume periodicity
//
// s(j,l): return the value (for reading)
// sptr(j,l): return the pointer (for writing)
//
float s(int j, int l) 
{
	//
	// to handle wrap around: 
	//   number of data in level j is 2^j 
	//   (already available in INCR)
	//
	l = (l+INCR[j])%INCR[j];
	return S(j,l);
}

float *sptr(int j, int l) 
{
	l = (l+INCR[j])%INCR[j];
	return &(S(j,l));
}
float d(int j, int l) 
{
	l = (l+INCR[j])%INCR[j];
	return D(j,l);
}

float *dptr(int j, int l) 
{
	l = (l+INCR[j])%INCR[j];
	return &(D(j,l));
}

//
// prepare data for plotting
//
void PutOnScreen(int j)
{
	double dx;
	int i;

	ndata = powf(2,j);
	dx = INCR[JMAX-j];
	for (i = 0; i < ndata; i++) {
		y[i] = s(j,i);
		x[i] = i*dx;
	}
}

void RestoreData ()
{
	int i;
	for (i = 0; i < ndata; i++) 
		ss[i] = sold[i];
}

void ShowSignal ()  // ... original input
{
	double dx;
	int i;

	ndata = powf(2,JMAX);
	dx = 1;
	for (i = 0; i < ndata; i++) {
		y[i] = sold[i];  // original signal
		x[i] = i*dx;
	}
}

float RMSError ()
{
	int i;
	float sum = 0.0;

	for (i = 0; i < ndata; i++) 
		sum += (sold[i]-ss[i])*(sold[i]-ss[i]);
	sum /= ndata;
	return sqrt (sum);
}


float P(int j, int i)
{
	// linear interpolation (order = 2)
	return (s(j-1,i)+s(j-1,i+1))/2;
}

float U(int j, int i)
{
	// update (dual order = 2)
	return 0.25*d(j-1,i-1)+0.25*d(j-1,i);
}

//
// for the 4 functions below:
//
// my convention: 
//    the argument j is the level of the finer level
//
void Predictor (int j)
{
	int i;

	for (i = 0; i < INCR[j-1]; i++) 
		*dptr(j-1,i) -= P(j,i);
}

void Update (int j) 
{
	int i;

	for (i = 0; i < INCR[j-1]; i++) 
		*sptr(j-1,i) += U(j,i);
}

void UndoUpdate (int j)
{
	int i;
	for (i = 0; i < INCR[j-1]; i++) 
		*sptr(j-1,i) -= U(j,i);
}

void UndoPredict (int j)
{
	int i;
	for (i = 0; i < INCR[j-1]; i++) 
		*dptr(j-1,i) += P(j,i);
}


void ForwardTransform () 
{
	int j;
	for (j = JMAX; j > 0; j--) {
		Predictor (j);
		Update (j);
	}  
}

void InverseTransform ()
{
	int j;
	for (j = 1; j <= JMAX; j++) {
		UndoUpdate (j);
		UndoPredict (j);
	}
}

/*-------------- Graphics Stuff -------------------*/

int drawmode=GL_POINTS;

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
	glPointSize (2.0);
	InitSignal();
	PutOnScreen (JMAX);
	FindBound ();
	glShadeModel (GL_FLAT);
}



void display(void)
{
	int i;
	glClear (GL_COLOR_BUFFER_BIT);

	glColor3f (0.0, 0.0, 0.0);
	DrawAxes();

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
	static int level = JMAX;

	// restore data; show plot
	if (value == 5) {
		level = JMAX;
		RestoreData ();
		ShowSignal();
		glutPostRedisplay();
	}

	// forward + inverse + plot: check consistency with original
	if (value == 7) {  
		ForwardTransform();
		//InverseTransform();
		PutOnScreen (JMAX);
		glutPostRedisplay();
	}

	if (value == 77) {		//clear
		glClear (GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
	}

	if (value == 9)  
		exit(0);
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
	int mainmenu, submenu;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (400, 400); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	mainmenu=glutCreateMenu(main_menu);
	glutAddMenuEntry ("restore data", 5);
	glutAddMenuEntry("inversed", 7);
	glutAddMenuEntry("clear", 77);
	glutAddMenuEntry ("quit", 9);

	glutAddMenuEntry("----------",0);
	submenu = glutCreateMenu(draw_mode);
	glutAddMenuEntry("Points", 1);
	glutAddMenuEntry("Lines", 2);
	glutSetMenu (mainmenu);
	glutAddSubMenu("Draw mode", submenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}

