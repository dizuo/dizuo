
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"
#include "Color.h"

class VertexPointerSet;

class Vertex
{
public:

	double x;
	double y;
	double z;

	Color color;

	static int UNKNOWN;
	static int INSIDE;
	static int OUTSIDE;
	static int BOUNDARY;

	int status;

	static double TOL;

	Vertex();

	Vertex(const Vertex & otherVertex);
		
	Vertex(const Vector & position, const Color & color);
	Vertex(const Vector & position, const Color & color, int eStatus);
	
	Vertex(double x, double y, double z, const Color & color);
	Vertex(double x, double y, double z, const Color & color, int eStatus);

	virtual ~Vertex();
	
	//String toString();
	bool equals(Vertex * vertex);
		
	void setStatus(int eStatus);
	Vector getPosition();
	VertexPointerSet * getAdjacentVertices();
	
	int getStatus();

	Color getColor();

	void addAdjacentVertex(Vertex * adjacentVertex);

	void mark(int eStatus);

	Vertex & operator=(const Vertex & v);

private:

	VertexPointerSet * adjacentVertices;
};

#endif // VERTEX_H
