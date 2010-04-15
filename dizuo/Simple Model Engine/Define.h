#ifndef DEFINE_H_
#define DEFINE_H_
#include "../gtl/gtl.hpp"
#include "../gtl/vec2.hpp"
#include "../gtl/vec3.hpp"
#include "../gtl/quat.hpp"
#include "../gtl/ray.hpp"
#include "../gtl/box3.hpp"
#include "../gtl/plane.hpp"
#include "../gtl/matrix3.hpp"
#include "../gtl/matrix4.hpp"


#include <vector>
#include <stack>
#include <Windows.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/glut.h>

//////////////////////////////////////////////////////////////////////////
typedef float					Real;
typedef	gtl::Vec2f				RVec2;
typedef gtl::Vec3f				RVec3;
typedef gtl::Matrix4f			RMat4;
typedef gtl::Box3f				RBBox;
typedef gtl::Quatf				RQuat4;

typedef std::vector<gtl::Vec3f> RVec3Array;

typedef gtl::Vec3d				DVec3;
typedef gtl::Matrix4d			DMat4;
typedef gtl::Quatd				DQuat4;

const Real PI     = 3.14159265358979f;
const Real TWOPI  = 6.28318530717958f;
const Real PIDIV2 = 1.57079632679489f;

#define Pi				3.14159265
#define PiOver180		1.74532925199433E-002
#define PiUnder180		5.72957795130823E+001
#define SMALL_NUMBER	0.00001
#define LARGE_NUMBER	1E20

#define TIMER1	1
#define TIMER2	2

#define SAFE_DELETE_OBJECT(p) { if(p){delete p; p=0;} }
#define SAFE_DELETE_VECTOR(p) { if(p){delete[] p; p=0;} }

enum AXIS_PLANE
{
	XOY,
	XOZ,
	YOZ
};

enum DIRECTION
{
	CCW = 0,
	CW = 1
};
//////////////////////////////////////////////////////////////////////////

#endif