#ifndef _DKGL_PREREQUEST_H_
#define _DKGL_PREREQUEST_H_

#pragma warning (disable: 4244)

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>
#include <limits>
#include <string>
#include <cassert>
#include <iostream>

//#define DKGL_USE_GDI_PLUS

/** ifndef DKGL_ENABLE_LOG, the log system doesn't work. */
#define DKGL_ENABLE_LOG

namespace DKGL
{

typedef float Real;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef int FONTid;

static const Real PI = Real( 4.0 * atan( 1.0 ) );
static const Real TWO_PI = Real( 2.0 * PI );
static const Real HALF_PI = Real( 0.5 * PI );
static const Real DEG2RAD = PI / Real(180.0);
static const Real RAD2DEG = Real(180.0) / PI;
static const Real UNIT_RAND = Real(rand()) / Real(RAND_MAX);

static const int MAX_FONT = 10;

enum TRANS_SPACE{LOCAL, GLOBAL};
enum PROJ_MODE{PERSPECTIVE, ORTHO};

enum SHADING_MODE{
	SM_FLAT = GL_FLAT,
	SM_SMOOTH = GL_SMOOTH
};

enum SHADING_MODEL{
	SML_AMBIENT = GL_AMBIENT,
	SML_DIFFUSE = GL_DIFFUSE,
	SML_SPECULAR = GL_SPECULAR
};

enum RENDER_MODE{
	RM_POINT = GL_POINT, 
	RM_WIRE = GL_LINE, 
	RM_POLYGON = GL_FILL
};

enum FACE_TYPE{
	FA_FRONT = GL_FRONT, 
	FA_BACK = GL_BACK,
	FA_FRONT_AND_BACK = GL_FRONT_AND_BACK, 
};

enum ERROR_LEVEL{
	ERR_WARNING, 
	ERR_ERROR
};

enum FONT_TYPE{
	FT_DONTCARE = 0, 
	FT_NORMAL = 400, 
	FT_BOLD = 700,
	FT_ULTRABOLD = 800
};

enum MSG_ALIGNMENT{
	MA_ALIGN2LEFT = 0, 
	MA_ALIGN2CENTER = 1, 
	MA_ALIGN2RIGHT = 2
};

enum LIGHT_TYPE{
	LT_POINT = 0,
	LT_DIRECTION = 1
};


}
#endif