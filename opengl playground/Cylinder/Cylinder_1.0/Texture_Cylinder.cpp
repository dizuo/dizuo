#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include "Cylinder.h"

#include "BMPLoader.h"

/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

Cylinder		g_cylinder(0.5f, 1.3f, 30, 5);
static float	angleX = 0.0f;
static float	angleY = 0.0f;
//////////////////////////////////////////////////////////////////////////
void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
			checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}

void init(void)
{    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

 	//makeCheckImage();
	BMPClass bmp;
	BMPLoad("bmp16.bmp", bmp);

 	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
 	glGenTextures(1, &texName);
 	glBindTexture(GL_TEXTURE_2D, texName);
 
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
/*
 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
 		0, GL_RGBA, GL_UNSIGNED_BYTE, / *checkImage* /bmp.bytes);
*/
	glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);

	glPushMatrix();
	{
		//	position(x, y, z) = 0.5 0 0
		//	texture(u, v) = 0 0

		//	position(x, y, z) = -0.5 0 7.54979e-008
		//	texture(u, v) = 0.999999 0

		//	position(x, y, z) = 0.5 1.3 0
		//	texture(u, v) = 0 1

		//	position(x, y, z) = -0.5 1.3 7.54979e-008
		//	texture(u, v) = 0.999999 1

		glRotatef(angleX, 1.0f, 0.0f, 0.0f);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);
		g_cylinder.Render(WIRE);
/*
 		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0, 0.0);			glVertex3f(0.5, 0.0, 0.0);
 			glTexCoord2f(0.9999, 0.0);		glVertex3f(-0.5, 0.0, 7.54979e-008);
 			glTexCoord2f(0.9999, 1.0);		glVertex3f(-0.5, 1.3, 7.54979e-008);
			glTexCoord2f(0.0, 1.0);			glVertex3f(0.5, 1.3, 0.0);
		}glEnd();*/


	}glPopMatrix();

	glutSwapBuffers();	
	glDisable(GL_TEXTURE_2D);

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.6);
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	  case 27:
		  exit(0);
		  break;

	  case 'x':
		  angleX += 5.0f;
		  break;
	  case 's':
		  angleX -= 5.0f;

	  case 'y':
		  angleY += 4.0f;
		  break;

	  case 'h':
		  angleY -= 4.0f;
		  break;

	  default:
		  break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();


	return 0; 
}
