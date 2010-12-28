
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color();
	Color(const Color & col);
	Color(int red, int green, int blue);
	virtual ~Color();

	int red;
	int green;
	int blue;
};

#endif // COLOR_H
