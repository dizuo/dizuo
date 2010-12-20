/*
cjbackhouse@hotmail.com 		www.backhouse.tk

I would appreciate it if anyone using this in something cool would tell me
so I can see where it ends up.
*/
/*
Takes a filename, returns an array of RGB pixel data
  Loads:
  24bit bitmaps
  256 colour bitmaps
  16 colour bitmaps
  2 colour bitmaps  (Thanks to Charles Rabier)

  This code is designed for use in openGL programs, so bitmaps not correctly padded will not
  load properly, I believe this only applies to: 
  256cols if width is not a multiple of 4
  16cols if width is not a multiple of 8
  2cols if width is not a multiple of 32

  Sample code:

	BMPClass bmp;
	BMPLoad(fname,bmp);
	glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);
*/


/*
  This file is a very simple example of how to use my code.
  (If you don't have glut then just rewrite in your favourite OpenGL thingy)
  Please read BMPLoader.h for some documentation
*/
#include <iostream>

#include "BMPLoader.h"
#include <GL/glut.h>

void OnSize(int x,int y);
void OnDraw();

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("yrBMP");
	glutReshapeFunc(OnSize);
	glutDisplayFunc(OnDraw);

	//std::cout<<"Enter filename:";
	//std::string fname;
	//std::cin>>fname;
	//std::string fname("bmp16.bmp");
	std::string fname("NeHe.bmp");
	BMPClass bmp;
	BMPLoad(fname,bmp);
	
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);

	glutReshapeWindow(bmp.width,bmp.height);

	glutMainLoop();
	return 0;
}

void OnSize(int x,int y)
{
	glViewport(0,0,x,y);
	glDrawBuffer(GL_BACK);

	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,1,1,0);
	glMatrixMode(GL_MODELVIEW);
}

void OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
		glTexCoord2d(0,1);	glVertex2d(0,0);
		glTexCoord2d(1,1);	glVertex2d(1,0);
		glTexCoord2d(1,0);	glVertex2d(1,1);
		glTexCoord2d(0,0);	glVertex2d(0,1);
	glEnd();

	glutSwapBuffers();
}
