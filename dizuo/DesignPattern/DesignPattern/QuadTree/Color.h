#ifndef COLOR_H_
#define COLOR_H_

#include <gl/freeglut.h>

class Color4
{
	double mVec[4];
public:
	Color4()
	{
		mVec[0] = 0.0f;
		mVec[1] = 0.0f;
		mVec[2] = 0.0f;
		mVec[3] = 1.0f;
	}

	explicit Color4(GLuint rgb, GLubyte a=0xFF)
	{
		GLubyte  r = GLubyte ((rgb >> 16) & 0xFF);
		GLubyte  g = GLubyte ((rgb >> 8) & 0xFF);
		GLubyte  b = GLubyte (rgb & 0xFF);
		mVec[0] = r / 255.0f;
		mVec[1] = g / 255.0f;
		mVec[2] = b / 255.0f;
		mVec[3] = a / 255.0f;
	}
	
	explicit Color4(double x, double y, double z, double w=1) 
	{
		mVec[0] = x;
		mVec[1] = y;
		mVec[2] = z;
		mVec[3] = w;
	}

	const double r() const {return mVec[0];}
	const double g() const {return mVec[1];}
	const double b() const {return mVec[2];}
	const double a() const {return mVec[3];}

	double& r() {return mVec[0];}
	double& g() {return mVec[1];}
	double& b() {return mVec[2];}
	double& a() {return mVec[3];}

	Color4 operator=(const Color4& v) 
	{
		r() = v.r();
		g() = v.g();
		b() = v.b();
		a() = v.a();
		return v;
	}
};

#endif