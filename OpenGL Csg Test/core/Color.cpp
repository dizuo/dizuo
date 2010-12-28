
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Color.h"

Color::Color() : red(0), green(0), blue(0)
{
}

Color::Color(const Color & col) : red(col.red), green(col.green), blue(col.blue)
{
}

Color::Color(int red, int green, int blue) : red(red), green(green), blue(blue)
{
}

Color::~Color()
{
}
