
/* cubemap.c - a simple EXT_texture_cube_map example */

/* Copyright NVIDIA Corporation, 1999. */

/* Very simplistic cube map demo.  The cube map faces are each a different
solid color.  Then a sphere is drawn.

When correct, the middle patch should be blue.  The upper patch
should be green.  The lower patch should be yellow.  The left
patch should be magenta.  The right patch should be red.  The back
patch should be cyan and wrapped around the edge of the sphere as
a circle. */

/* Windows command line compile instructions:

cl cubemap.c glut32.lib

Unix command line compile instructions:

cc -o cubemap cubemap.c -lglut -lGLU -lGL -lXext -lX11 -lm

Note that this program requires the OpenGL Utility Toolkit (GLUT).
You can download GLUT from http://reality.sgi.com/opengl/glut3 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <gl/glext.h>

/* In case your <GL/gl.h> does not advertise EXT_texture_cube_map... */
#ifndef GL_EXT_texture_cube_map
# define GL_NORMAL_MAP_EXT                   0x8511
# define GL_REFLECTION_MAP_EXT               0x8512
# define GL_TEXTURE_CUBE_MAP_EXT             0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP_EXT     0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT  0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT  0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT  0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT  0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT  0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT  0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP_EXT       0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT    0x851C
#endif

/* constants */
#ifndef M_PI
#define M_PI            (3.14159265358979f)
#endif
#define DTOR            (M_PI/180.0)
#define RTOD            (180.0/M_PI)

#define CUBE_MAP_SIZE 256

#define ZTRANS 4.0F

int exitAfterOneFrame = 0;

enum {CUBE_POS_X, CUBE_NEG_X, CUBE_POS_Y, CUBE_NEG_Y, CUBE_POS_Z, CUBE_NEG_Z};
static unsigned char CubeMap[6][CUBE_MAP_SIZE][CUBE_MAP_SIZE][4];
static void InitGraphics(void);
static void light_state(long, long);
static void DrawSphere(float);
static void vert(float, float);

static void
vert(float theta, float phi)
{
	float r = 0.75f;
	float x, y, z, nx, ny, nz;

	nx = sin(DTOR * theta) * cos(DTOR * phi);
	ny = sin(DTOR * phi);
	nz = cos(DTOR * theta) * cos(DTOR * phi);
	glNormal3f(nx, ny, nz);

	x = r * sin(DTOR * theta) * cos(DTOR * phi);
	y = r * sin(DTOR * phi);
	z = -ZTRANS + r * cos(DTOR * theta) * cos(DTOR * phi);
	glVertex4f(x, y, z, 1.0);
}

static void
DrawSphere(float del)
{
	float phi, phi2, theta;

	glColor4f(1.0, 1.0, 1.0, 1.0);
	for (phi = -90.0f; phi < 90.0f; phi += del) {
		glBegin(GL_TRIANGLE_STRIP);

		phi2 = phi + del;

		for (theta = -90.0f; theta <= 90.0f; theta += del) {
			vert(theta, phi);
			vert(theta, phi2);
		}
		glEnd();
	}
}

static float
Dot3(float *a, float *b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static float *
Scale3(float *result, float *a, float scale)
{
	result[0] = a[0] * scale;
	result[1] = a[1] * scale;
	result[2] = a[2] * scale;
	return result;
}

static float *
Normalize3(float *result, float *a)
{
	float length;

	length = (float) sqrt(Dot3(a, a));
	return Scale3(result, a, 1 / length);
}

static unsigned char *
CubeFunc(unsigned char resultColor[3], float vec[3])
{
	int i;
	float faceVec[3];

	if (vec[0] == 1.0) {
		resultColor[0] = 255;
		resultColor[1] = 0;
		resultColor[2] = 0;
	} else if (vec[1] == 1.0) {
		resultColor[0] = 0;
		resultColor[1] = 255;
		resultColor[2] = 0;
	} else if (vec[2] == 1.0) {
		resultColor[0] = 0;
		resultColor[1] = 0;
		resultColor[2] = 255;
	} else if (vec[0] == -1.0) {
		resultColor[0] = 255;
		resultColor[1] = 0;
		resultColor[2] = 255;
	} else if (vec[1] == -1.0) {
		resultColor[0] = 255;
		resultColor[1] = 255;
		resultColor[2] = 0;
	} else if (vec[2] == -1.0) {
		resultColor[0] = 0;
		resultColor[1] = 255;
		resultColor[2] = 255;
	}
	return resultColor;

	Normalize3(faceVec, vec);
	for (i = 0; i < 3; i++) {
		resultColor[i] = 255 * (sin(6 * (faceVec[i] + faceVec[(i + 1) % 3])) + 1) / 2.0;
	}
	return resultColor;
}

GLenum cubefaces[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT,
};

static void 
InitGraphics(void)
{
	int i, j, k;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -0.75, 0.75, ZTRANS - 1.0, ZTRANS + 1.0); // match 
	// 640x480

	glMatrixMode(GL_MODELVIEW);;
	glLoadIdentity();

	for (i = 0; i < CUBE_MAP_SIZE; i++) {
		float t = 1.0 / (2 * CUBE_MAP_SIZE) + (float) i / CUBE_MAP_SIZE;
		t = 2.0 * t - 1.0;
		for (j = 0; j < CUBE_MAP_SIZE; j++) {
			float s = 1.0 / (2 * CUBE_MAP_SIZE) + (float) j / CUBE_MAP_SIZE;
			float pt[3];
			s = 2.0 * s - 1.0;
			pt[0] = 1;
			pt[1] = t;
			pt[2] = -s;
			CubeFunc(CubeMap[CUBE_POS_X][i][j], pt);
			pt[0] = -1;
			pt[1] = t;
			pt[2] = s;
			CubeFunc(CubeMap[CUBE_NEG_X][i][j], pt);

			pt[1] = 1;
			pt[0] = s;
			pt[2] = -t;
			CubeFunc(CubeMap[CUBE_POS_Y][i][j], pt);
			pt[1] = -1;
			pt[0] = s;
			pt[2] = t;
			CubeFunc(CubeMap[CUBE_NEG_Y][i][j], pt);

			pt[2] = 1;
			pt[0] = s;
			pt[1] = t;
			CubeFunc(CubeMap[CUBE_POS_Z][i][j], pt);
			pt[2] = -1;
			pt[0] = -s;
			pt[1] = t;
			CubeFunc(CubeMap[CUBE_NEG_Z][i][j], pt);
			for (k = CUBE_POS_X; k <= CUBE_NEG_Z; k++) {
				CubeMap[k][i][j][3] = 255;
			}
		}
	}

	glEnable(GL_DEPTH_TEST);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	for (i = 0; i < 6; i++) {
		glTexImage2D(
			cubefaces[i],
			0,                  // level
			GL_RGBA8,          // internal format
			CUBE_MAP_SIZE,     // width
			CUBE_MAP_SIZE,     // height
			0,                 // border
			GL_RGBA,           // format
			GL_UNSIGNED_BYTE,   // type
			CubeMap[CUBE_POS_X + i]); // pixel data
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_NORMALIZE);
}

static void 
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawSphere(9.0);

	glutSwapBuffers();
	if (exitAfterOneFrame) {
		exit(0);
	}
}

static void
keyboard(unsigned char c, int x, int y)
{
	switch (c) {
  case 27:
	  exit(0);
	  break;
  case ' ':  /* Space bar redraws. */
	  glutPostRedisplay();
	  break;
	}
}

int
main(int argc, char **argv)
{
	int i;

	glutInitWindowSize(640, 480);
	glutInit(&argc, argv);
	for (i = 1; i < argc; i++) {
		if (!strcmp("-e", argv[i])) {
			exitAfterOneFrame = 1;
		}
	}
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("cubemap");

	/* Run-time extension check. */
	if (!glutExtensionSupported("GL_EXT_texture_cube_map")) {
		fprintf(stderr,
			"cubemap: Requires the EXT_texture_cube_map OpenGL extension to run.\n");
		exit(0);
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	InitGraphics();
	glutMainLoop();
	return 0;
}