#include "ColorCube.h"

#include <gl/freeglut.h>

GLfloat ColorCube::vertices[24] = {
	-1.0,-1.0,-1.0, 1.0,-1.0,-1.0, 1.0,1.0,-1.0, -1.0,1.0,-1.0,	//all z==-1.0   
	-1.0,-1.0,1.0,  1.0,-1.0,1.0,  1.0,1.0,1.0,  -1.0,1.0,1.0	//all z==1.0
};

GLfloat ColorCube::colors[24] = {
	0.0,0.0,0.0, 1.0,0.0,0.0, 1.0,1.0,0.0, 0.0,1.0,0.0,			//all blue==0.0
	0.0,0.0,1.0, 1.0,0.0,1.0, 1.0,1.0,1.0, 0.0,1.0,1.0			//all blue==1.0
};

bool ColorCube::isColoring = false;

void ColorCube::renderColorCube(const Box3f& box)
{
	float x0 = box.getMin().x();
	float y0 = box.getMin().y();
	float z0 = box.getMin().z();
	float x1 = box.getMax().x();
	float y1 = box.getMax().y();
	float z1 = box.getMax().z();

	GLfloat temp[] = {		//修改顶点数据
		x0, y0, z0,  x1, y0, z0,  x1, y1, z0,  x0, y1, z0,	//all z == z0
		x0, y0, z1,  x1, y0, z1,  x1, y1, z1,  x0, y1, z1	//all z == z1
	};

	for (size_t i(0); i<24; i++)
		vertices[i] = temp[i];

	renderColorCube();
}

void ColorCube::renderColorCube()
{
	/*map vertices to faces*/
	Polygon(1,0,3,2,0);				//Bottom
	Polygon(3,7,6,2,1);				//Left
	Polygon(7,3,0,4,2);				//Front

	Polygon(2,6,5,1,3);				//Back
	Polygon(4,5,6,7,4);				//Top
	Polygon(5,4,0,1,5);				//Right
}

void ColorCube::Polygon(int a, int b, int c, int d, int face)
{
	/*Draw a polygon via list of vertices*/
	a = a * 3; 	b = b * 3; 	c = c * 3; 	d = d * 3;

	glBegin(GL_LINE_STRIP);		//glBegin(GL_POLYGON);
	if(isColoring)	
		glColor3f(colors[a], colors[a+1], colors[a+2]);
	glVertex3f(vertices[a], vertices[a+1], vertices[a+2]);

	if(isColoring)
		glColor3f(colors[b], colors[b+1], colors[b+2]);
	glVertex3f(vertices[b], vertices[b+1], vertices[b+2]);

	if(isColoring)
		glColor3f(colors[c], colors[c+1], colors[c+2]);
	glVertex3f(vertices[c], vertices[c+1], vertices[c+2]);

	if(isColoring)
		glColor3f(colors[d], colors[d+1], colors[d+2]);
	glVertex3f(vertices[d], vertices[d+1], vertices[d+2]);
	
	if(isColoring)
		glColor3f(colors[a], colors[a+1], colors[a+2]);
	glVertex3f(vertices[a], vertices[a+1], vertices[a+2]);
	glEnd();
}
