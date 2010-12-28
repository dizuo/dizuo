
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include <stdlib.h>

#include "Line.h"

#include "Face.h"
#include "Vertex.h"

#include <math.h>

//double Line::TOL = 1e-10f;
double Line::TOL = 1e-5f;

Line::Line()
{
	point = VectorZero;
	direction = Vector(1,0,0);
}

Line::Line(Face * face1, Face * face2)
{
	Vector normalFace1 = face1->getNormal();
	Vector normalFace2 = face2->getNormal();
	
	//direction: cross product of the faces normals
	
	direction = VectorCross(normalFace1, normalFace2);

	//double TOL = 0.00001f;
			
	//if direction lenght is not zero (the planes aren't parallel )...
	if (!(direction.Magnitude()<TOL))
	{
		//getting a line point, zero is set to a coordinate whose direction 
		//component isn't zero (line intersecting its origin plan)
		double d1 = -(normalFace1.x*face1->v1->x + normalFace1.y*face1->v1->y + normalFace1.z*face1->v1->z);
		double d2 = -(normalFace2.x*face2->v1->x + normalFace2.y*face2->v1->y + normalFace2.z*face2->v1->z);
		if(fabs(direction.x)>TOL)
		{
			point.x = 0;
			point.y = (d2*normalFace1.z - d1*normalFace2.z)/direction.x;
			point.z = (d1*normalFace2.y - d2*normalFace1.y)/direction.x;
		}
		else if(fabs(direction.y)>TOL)
		{
			point.x = (d1*normalFace2.z - d2*normalFace1.z)/direction.y;
			point.y = 0;
			point.z = (d2*normalFace1.x - d1*normalFace2.x)/direction.y;
		}
		else
		{
			point.x = (d2*normalFace1.y - d1*normalFace2.y)/direction.z;
			point.y = (d1*normalFace2.x - d2*normalFace1.x)/direction.z;
			point.z = 0;
		}
	}
			
	direction.Normalise();
}

Line::Line(const Vector & directioni, const Vector & pointi)
{
	direction = directioni;
	point = pointi;
	direction.Normalise();
}

Vector Line::getPoint()
{
	return point;
}

Vector Line::getDirection()
{
	return direction;
}

void Line::setPoint(const Vector & pointi)
{
	point = pointi;
}

void Line::setDirection(const Vector & directioni)
{
	direction = directioni;
}

double Line::computePointToPointDistance(const Vector & otherPoint)
{
	double distance = (point - otherPoint).Magnitude();
	Vector vec = Vector(otherPoint.x - point.x, otherPoint.y - point.y, otherPoint.z - point.z);
	vec.Normalise();
	if ((vec * direction)<0)
	{
		return -distance;			
	}
	else
	{
		return distance;
	}
}

Vector Line::computeLineIntersection(Line * otherLine)
{
	//x = x1 + a1*t = x2 + b1*s
	//y = y1 + a2*t = y2 + b2*s
	//z = z1 + a3*t = z2 + b3*s
	
	Vector linePoint = otherLine->getPoint(); 
	Vector lineDirection = otherLine->getDirection();

	//double TOL = 0.00001;
			
	double t;
	if(fabs(direction.y*lineDirection.x-direction.x*lineDirection.y)>TOL)
	{
		t = (-point.y*lineDirection.x+linePoint.y*lineDirection.x+lineDirection.y*point.x-lineDirection.y*linePoint.x)/(direction.y*lineDirection.x-direction.x*lineDirection.y);
	}
	else if (fabs(-direction.x*lineDirection.z+direction.z*lineDirection.x)>TOL)
	{
		t=-(-lineDirection.z*point.x+lineDirection.z*linePoint.x+lineDirection.x*point.z-lineDirection.x*linePoint.z)/(-direction.x*lineDirection.z+direction.z*lineDirection.x);
	}
	else if (fabs(-direction.z*lineDirection.y+direction.y*lineDirection.z)>TOL)
	{
		t = (point.z*lineDirection.y-linePoint.z*lineDirection.y-lineDirection.z*point.y+lineDirection.z*linePoint.y)/(-direction.z*lineDirection.y+direction.y*lineDirection.z);
	}
	else
	{
		return VectorZero;
		//return null;
	}

	Vector vResult = point + direction * t;
	
	return vResult;
}

Vector Line::computePlaneIntersection(const Vector & normal, const Vector & planePoint, bool & bResult)
{
	bResult = true;

	//Ax + By + Cz + D = 0
	//x = x0 + t(x1 – x0)
	//y = y0 + t(y1 – y0)
	//z = z0 + t(z1 – z0)
	//(x1 - x0) = dx, (y1 - y0) = dy, (z1 - z0) = dz
	//t = -(A*x0 + B*y0 + C*z0 )/(A*dx + B*dy + C*dz)

	//double TOL = 0.00001f;
	
	double A = normal.x;
	double B = normal.y;
	double C = normal.z;
	double D = (normal * planePoint) * -1.0f;
		
	double numerator = (normal * point) + D;
	double denominator = (normal * direction);
			
	//if line is paralel to the plane...
	if(fabs(denominator)<TOL)
	{
		//if line is contained in the plane...
		if(fabs(numerator)<TOL)
		{
			return point;
		}
		else
		{
			bResult = false;
			return VectorZero;

			//return null;
		}
	}
	//if line intercepts the plane...
	else
	{
		double t = -numerator/denominator;
		Vector resultPoint = point + direction * t;
		
		return resultPoint;
	}
}

double LineRandomNum()
{
	int nRand = rand() % 10000;
	double fRand = (double)nRand;
	fRand *= 0.0001;
	fRand *= 2.0f;
	fRand -= 1.0f;
	return fRand;
}

void Line::perturbDirection()
{
	//direction.x += LineRandomNum();			
	//direction.y += LineRandomNum();			
	//direction.z += LineRandomNum();

	direction.x += LineRandomNum() * 0.001f;			
	direction.y += LineRandomNum() * 0.001f;			
	direction.z += LineRandomNum() * 0.001f;

	//direction.Normalise();
}

