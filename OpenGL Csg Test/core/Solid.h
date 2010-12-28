
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef SOLID_H
#define SOLID_H

#include "Color.h"

#include "IntSet.h"
#include "VectorSet.h"
#include "ColorSet.h"

#include <string>

class Solid
{
public:

	IntSet indices;
	VectorSet vertices;
	ColorSet colors;
	
	Solid(const std::string & sFileName, const Color & colBase, int dRed, int dGreen, int dBlue);
	Solid(VectorSet * vertices, IntSet * indices, ColorSet * colors);
	virtual ~Solid();
	
	VectorSet * getVertices();
	IntSet * getIndices();
	ColorSet * getColors();

	bool isEmpty();

	void setData(VectorSet & vertices, IntSet & indices, ColorSet & colors);
	
	void setColor(const Color & color);

	void Translate(const Vector & t);
	void Rotate(const Vector & a, const Vector & b, float angle);
	void Scale(const Vector & s);

	void Render();

	void loadCoordinateFile(const std::string & sFileName, const Color & colBase, int dRed, int dGreen, int dBlue);

	Vector getMean();
};

#endif // SOLID_H
