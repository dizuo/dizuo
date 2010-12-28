
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef BOUND_H
#define BOUND_H

#include "Vector.h"

class VectorSet;

class Bound
{
public:

	double xMax;
	double xMin;
	double yMax;
	double yMin;
	double zMax;
	double zMin;
	
	Bound(const Vector & p1, const Vector & p2, const Vector & p3);
	Bound(VectorSet * vertices);
	virtual ~Bound();
	
	bool overlap(Bound & bound);

	static double TOL;

private:

	void checkVertex(const Vector & vertex);
};

#endif // BOUND_H
