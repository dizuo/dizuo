/*BMPLoader - loads Microsoft .bmp format
    Copyright (C) 2006  Chris Backhouse

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


  cjbackhouse@hotmail.com 		www.backhouse.tk
  
  I would appreciate it if anyone using this in something cool would tell me
  so I can see where it ends up.

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
#include "GL/glut.h"
#include <iostream>

#include "BMPLoader.h"

void OnSize(int x,int y);
void OnDraw();

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("yrBMP");
	glutReshapeFunc(OnSize);
	glutDisplayFunc(OnDraw);

	std::cout<<"Enter filename:";
	std::string fname;
	std::cin>>fname;
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
