#include <GL/glut.h>
/*---------------------------------------------------------------------------------------------*/
/*	Draw the cube
*/
GLfloat vertices[][3]={
	{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},{1.0,1.0,-1.0},{-1.0,1.0,-1.0},	//all z==-1.0   
	{-1.0,-1.0,1.0},{1.0,-1.0,1.0},{1.0,1.0,1.0},{-1.0,1.0,1.0}			//all z==1.0
};
GLfloat colors[][3]={
	{0.0,0.0,0.0},{1.0,0.0,0.0},{1.0,1.0,0.0},{0.0,1.0,0.0},			//all blue==0.0
	{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}				//all blue==1.0
};

void Polygon(int a, int b, int c, int d, int face)
{
	/*Draw a polygon via list of vertices*/
	//glBegin(GL_POLYGON);
	glBegin(GL_LINE_STRIP);	
	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);

	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);
	glEnd();
}	
/*---------------------------------------------------------------------------------------------*/
//			  6--------------5
//		(4)->/|				/|
//			/		(3)	   / |	
//		   7--|------------4 |
//		   |  |	  (2)	   | |<--(5)
//	(1)--> |  |			   | |	
//		   | 2/------------|-/1
//		   | /	 (0)	   |/
//		   |/--------------/
//		   3			  0	
void colorCube(void)
{
	/*map vertices to faces*/
	Polygon(1,0,3,2,0);				//Bottom
	Polygon(3,7,6,2,1);				//Left
	Polygon(7,3,0,4,2);				//Front

	Polygon(2,6,5,1,3);				//Back
	Polygon(4,5,6,7,4);				//Top
	Polygon(5,4,0,1,5);				//Right
}

/*---------------------------------------------------------------------------------------------*/