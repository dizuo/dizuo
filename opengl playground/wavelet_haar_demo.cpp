//
// Example to Assignment one 
// Introduction to Wavelet, Spring 2001
// 
// Remark: only implement Haar, one-level
//         you need to add more filters, multi-level
//
// copyright: Jyun-Ming Chen
//
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>


#define DATA 600 // maximum data
int ndata;       // actual data points in use
float x[DATA], y[DATA]; // data for plotting
float S[DATA], SS[DATA], tmp[DATA];  // S: original; SS: transformed

double dx;

//
// signal initialization
//
void SetData()
{
	int i;
	ndata = 32;   // 2^n
	for (i = 0; i < ndata; i++) {
		S[i] = (i+2)%7;
		printf ("%f\n", S[i]);
	}
}
//
// prepare data for plotting
//
void PutOnScreen()   
{
	double dx=1.0;
	int i;

	for (i = 0; i < ndata; i++) {
		y[i] = S[i];
		x[i] = i*dx;
	}
}
//
// perodic data access
//
float s(int i) {
	return S[(i+ndata)%ndata];
}
float ss(int i) {
	return SS[(i+ndata)%ndata];
}

float *sptr(int i) {
	return &(S[(i+ndata)%ndata]);
}
float *ssptr(int i) {
	return &(SS[(i+ndata)%ndata]);
}

//
// Filter Definitions
//
float h2[8], g2[8];
float ht[8], gt[8];
int hmin, hmax, hoffset, htmin, htmax, htoffset;
int gmin, gmax, goffset, gtmin, gtmax, gtoffset;

// ... to allow negative indices ...
float HT(int n) {
	return ht[n+htoffset];
}

float H2(int n) {
	return h2[n+hoffset];
}

float GT(int n) {
	return gt[n+gtoffset];
}

float G2(int n) {
	return g2[n+goffset];
}

void InitHaarFilters()
{
	// synthesis filter definition (Haar)
	h2[0] = 1.0;   
	h2[1] = 1.0; 
	hmin = 0; hmax = 1; hoffset = 0;
	g2[0] = 1.0;   
	g2[1] = -1.0; 
	gmin = 0; gmax = 1; goffset = 0;

	// h_tilda = h(-n): analysis filter
	ht[0] = 0.5;  // HT(-1)
	ht[1] = 0.5;  // HT( 0)
	htmin = -1; htmax = 0; htoffset = 1;  // such that HT(n) = ht[n+OFFSET]
	gt[0] = -0.5;  // GT(-1)
	gt[1] = 0.5;  // GT( 0)
	gtmin = -1; gtmax = 0; gtoffset = 1;  // such that GT(n) = gt[n+OFFSET]

}

void InitD4Filters()
{
	// add your stuff here ...
}

//
// Analysis:
//
// S: original data a(1,n)
// SS: coarsened a(0,n) + detailed b(0,n)
//
void analysis() {  
	int n, m;
	float sum;

	// forward transform: a(0, n)
	for (n= 0; n < ndata/2; n++) {
		sum = 0;
		for (m = 2*n-htmax; m <= 2*n-htmin; m++) {
			sum += s(m)*HT(2*n-m);
		}
		*ssptr(n) = sum;
	}	

	for (n= 0; n < ndata/2; n++) {
		sum = 0;
		for (m = 2*n-htmax; m <= 2*n-htmin; m++) {
			sum += s(m)*GT(2*n-m);
		}
		*ssptr(n+ndata/2) = sum;
	}	

	// debugging output
	for (n = 0; n < ndata; n++)
		printf ("%f\n", ss(n));
}   

//
// Synthesis:
//
// SS: coarsened a(0,n) + detailed b(0,n)
// S: (recovered) data a(1,n)
//
void synthesis () 
{
	int i, n, k;
	float sum;

	//
	// subdivision part
	//
	// SS: copy a(0,n) [1st half of SS] back to S
	//     copy b(0,n) [2nd half of SS] to tmp
	for (i = 0; i < ndata/2; i++)
		S[i] = SS[i];
	for (i = ndata/2; i < ndata; i++)
		tmp[i-ndata/2] = SS[i];

	// S: upsampling by two to SS
	for (i = 0; i < ndata; i++) {
		if (i%2)  // i is odd; insert zero
			SS[i] = 0;
		else
			SS[i] = S[i/2];
	}

	// convolve SS with H; result to S
	for (n = 0; n < ndata; n++) {
		sum = 0.0;
		for (k = n-hmax; k <= n-hmin; k++) {
			sum += ss(k)*H2(n-k);
		}
		*sptr(n) = sum;
	}

	//
	// detailed part
	//
	// b(0,n): upsampling by two to SS
	for (i = 0; i < ndata; i++) {
		if (i%2)  // i is odd; insert zero
			SS[i] = 0;
		else
			SS[i] = tmp[i/2];   // b(0,n)
	}

	// convolve SS with G; result ADD to S
	for (n = 0; n < ndata; n++) {
		sum = 0.0;
		for (k = n-gmax; k <= n-gmin; k++) {
			sum += ss(k)*G2(n-k);
		}
		*sptr(n) += sum;
	}


	for (i = 0; i < ndata; i++) 
		printf ("%lf\n", S[i]);
}

/*-------------- Graphics Stuff -------------------*/

int drawmode=2;
float max[2], min[2];
//
// locate the points so that they appear on window
//
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
//
// things to be done at initialization
//
void init(void) 
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glLineWidth (2.0);

	drawmode = GL_POINTS;
	glPointSize (2.0);

	InitHaarFilters ();    // initialize filters
	analysis();            // perform 1-level analysis
	synthesis();           // perform 1-level synthesis
	PutOnScreen();
	FindBound ();          
	glShadeModel (GL_FLAT);
}

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


void DrawAxes ()
{
#define drawOneLine(x1,y1,x2,y2)  glBegin(GL_LINES);  \
	glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();
#define Min(x,y) ((x) < (y) ? (x) : (y))

	drawOneLine (min[0], 0.0, max[0], 0.0);    // x-axis
	drawOneLine (min[0], Min(0,min[1]), min[0], max[1]);  // y-axis
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
	int i;
	if (value == 666)
		exit(0);
	if (value == 777) {
		SetData();
		analysis();
		for (i = 0; i < ndata/2; i++)
			SS[i+ndata/2] = 0.0;
		synthesis();
		PutOnScreen();
		glutPostRedisplay();
	}
	if (value == 888) {
		SetData();
		PutOnScreen();
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
	glutAddMenuEntry("zero all detail coeff.", 777);
	glutAddMenuEntry("restore data", 888);
	glutAddSubMenu("Draw mode", submenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}

