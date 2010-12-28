
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef LINE_H
#define LINE_H

#include "Vector.h"
#include "Face.h"

class Line
{
public:

	Vector point;
	Vector direction;
	
	static double TOL;

	Line();

	Line(Face * face1, Face * face2);
	Line(const Vector & direction, const Vector & point);
	
	Vector getPoint();
	Vector getDirection();
	void setPoint(const Vector & point);
	void setDirection(const Vector & direction);
		
	double computePointToPointDistance(const Vector & otherPoint);
	
	Vector computeLineIntersection(Line * otherLine);
	Vector computePlaneIntersection(const Vector & normal, const Vector & planePoint, bool & bResult);

	void perturbDirection();
};

#endif // LINE_H
