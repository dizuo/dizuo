#ifndef COLORCUBE_H_
#define COLORCUBE_H_

#include "Box3.hpp"

class ColorCube
{
public:
	static void renderColorCube(const Box3f& box);
	static void renderColorCube();
	static void activeColor() { isColoring = true; }
	static void deactiveColor() { isColoring = false; }
protected:
	static void Polygon(int a, int b, int c, int d, int face);
	static float vertices[24];
	static float colors[24];
	static bool  isColoring;
};

//////////////////////////////////////////////////////////////////////////
// 4/20/2009	By : RenYaFei
#endif