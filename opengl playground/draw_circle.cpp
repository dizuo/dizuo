#include <GL/glut.h>
class screenPt
{
private:
	int x,y;
public:
	screenPt(){
		x=y=0;
	}
	void setCoords(int xCoordValue,int yCoordValue){
		x=xCoordValue;
		y=yCoordValue;
	}
	int getx() {
		return x;
	}
	int gety() {
		return y;
	}
	void incrementx(){
		x++;
	}
	void decrementy(){
		y--;
	}

};
void init (void)
{
	//glClearColor (0.5, 1.0, 0.5, 0.0); // Set display-window color to white.
	glClearColor(0, 0, 0, 0);
	glMatrixMode (GL_PROJECTION); // Set projection parameters.

	// 正交投影到窗口，原点位于窗口左下角
	gluOrtho2D (0, 200.0, 0, 150.0);
}

void setPixel(int xCoord,int yCoord)
{
	glBegin(GL_POINTS);
		glVertex2i(xCoord,yCoord);
	glEnd();
}

void circleMidpoint(int xc,int yc,int radius)
{   
	screenPt circPt;
	int p=1-radius;
	circPt.setCoords(0,radius);
	void circlePlotPoints(GLint,GLint,screenPt);
	circlePlotPoints(xc,yc,circPt);
	while(circPt.getx()<circPt.gety()){
		circPt.incrementx();
		if(p<0)
			p+=2*circPt.getx()+1;
		else{
			circPt.decrementy();
			p+=2*(circPt.getx()-circPt.gety())+1;
		}
		circlePlotPoints(5,5,circPt);
	}
}
void drawcircle (void)
{
	glClear (GL_COLOR_BUFFER_BIT); // Clear display window.
	glColor3f (0.0, 0.0, 1.0); // Set line segment color to red.
	
	circleMidpoint(0,0,30);		//半径稍微大点，才能看到~
	//glRecti(0, 0, 100, 100);

	glFlush (); // Process all OpenGL routines as quickly as possible.
}



void circlePlotPoints(int xc,int yc,screenPt circPt)
{
	setPixel(xc+circPt.getx(),yc+circPt.gety());
	setPixel(xc-circPt.getx(),yc+circPt.gety());
	setPixel(xc+circPt.getx(),yc-circPt.gety());
	setPixel(xc-circPt.getx(),yc-circPt.gety());
	setPixel(xc+circPt.gety(),yc+circPt.getx());
	setPixel(xc+circPt.gety(),yc+circPt.getx());
	setPixel(xc+circPt.gety(),yc-circPt.getx());
	setPixel(xc+circPt.gety(),yc-circPt.getx());
}

void main (int argc, char** argv)
{  
	glutInit (&argc, argv); // Initialize GLUT.
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // Set display mode.
	glutInitWindowPosition (50, 100); // Set top-left display-window position.
	glutInitWindowSize (400, 300); // Set display-window width and height.
	glutCreateWindow ("An Example OpenGL Program"); // Create display window.
	init (); // Execute initialization procedure.
	glutDisplayFunc (drawcircle); // Send graphics to display window.
	glutMainLoop ( ); // Display everything and wait.
}
