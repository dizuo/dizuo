///*
// * Copyright (c) 1993-2003, Silicon Graphics, Inc.
// * All Rights Reserved
// *
// * Permission to use, copy, modify, and distribute this software for any
// * purpose and without fee is hereby granted, provided that the above
// * copyright notice appear in all copies and that both the copyright
// * notice and this permission notice appear in supporting documentation,
// * and that the name of Silicon Graphics, Inc. not be used in
// * advertising or publicity pertaining to distribution of the software
// * without specific, written prior permission.
// *
// * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS" AND
// * WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
// * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
// * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
// * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
// * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
// * OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION, LOSS OF
// * PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD
// * PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF
// * THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF
// * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE
// * OR PERFORMANCE OF THIS SOFTWARE.
// *
// * US Government Users Restricted Rights 
// * Use, duplication, or disclosure by the Government is subject to
// * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
// * (c)(1)(ii) of the Rights in Technical Data and Computer Software
// * clause at DFARS 252.227-7013 and/or in similar or successor clauses
// * in the FAR or the DOD or NASA FAR Supplement.  Unpublished - rights
// * reserved under the copyright laws of the United States.
// *
// * Contractor/manufacturer is:
// *	Silicon Graphics, Inc.
// *	1500 Crittenden Lane
// *	Mountain View, CA  94043
// *	United State of America
// *
// * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
// */
//
///*
// * hello.c
// * This is a simple, introductory OpenGL program.
// */
//#include <GL/glut.h>
////#include <stdlib.h>
//#define length 10
//void
//drawaxes(void)
//{
//	glScalef(0.5,0.5,0.5);
//	glTranslatef(1,1,0);
//	glColor3ub(255, 0, 0);
//	glBegin(GL_LINE_STRIP);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(1.0, 0.0, 0.0);
//	glVertex3f(0.75, 0.25, 0.0);
//	glVertex3f(0.75, -0.25, 0.0);
//	glVertex3f(1.0, 0.0, 0.0);
//	glVertex3f(0.75, 0.0, 0.25);
//	glVertex3f(0.75, 0.0, -0.25);
//	glVertex3f(1.0, 0.0, 0.0);
//	glEnd();
//	glBegin(GL_LINE_STRIP);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(0.0, 1.0, 0.0);
//	glVertex3f(0.0, 0.75, 0.25);
//	glVertex3f(0.0, 0.75, -0.25);
//	glVertex3f(0.0, 1.0, 0.0);
//	glVertex3f(0.25, 0.75, 0.0);
//	glVertex3f(-0.25, 0.75, 0.0);
//	glVertex3f(0.0, 1.0, 0.0);
//	glEnd();
//	glBegin(GL_LINE_STRIP);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(0.0, 0.0, 1.0);
//	glVertex3f(0.25, 0.0, 0.75);
//	glVertex3f(-0.25, 0.0, 0.75);
//	glVertex3f(0.0, 0.0, 1.0);
//	glVertex3f(0.0, 0.25, 0.75);
//	glVertex3f(0.0, -0.25, 0.75);
//	glVertex3f(0.0, 0.0, 1.0);
//	glEnd();
//
//	glColor3ub(255, 255, 0);
//	glRasterPos3f(1.1, 0.0, 0.0);
//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
//	glRasterPos3f(0.0, 1.1, 0.0);
//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
//	glRasterPos3f(0.0, 0.0, 1.1);
//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');
//}
//void display(void)
//{
///* clear all pixels  */
//   glClear (GL_COLOR_BUFFER_BIT);
//
///* draw white polygon (rectangle) with corners at
// * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)  
// */
//   //glColor3f (1.0, 1.0, 1.0);
//   //glBegin(GL_POLYGON);
//   //   glVertex3f (0.25, 0.25, 0.0);
//   //   glVertex3f (0.75, 0.25, 0.0);
//   //   glVertex3f (0.75, 0.75, 0.0);
//   //   glVertex3f (0.25, 0.75, 0.0);
//   //glEnd();
//   glPushMatrix();
//   glColor3ub(0, 0, 255);
//   glBegin(GL_LINE_STRIP);
//   glVertex3f(0.0, 0.0, 0.0);
//   glVertex3f(0.0, 0.0, -1.0*length);
//   glVertex3f(0.1, 0.0, -0.9*length);
//   glVertex3f(-0.1, 0.0, -0.9*length);
//   glVertex3f(0.0, 0.0, -1.0*length);
//   glVertex3f(0.0, 0.1, -0.9*length);
//   glVertex3f(0.0, -0.1, -0.9*length);
//   glVertex3f(0.0, 0.0, -1.0*length);
//   glEnd();
//   glColor3ub(255, 255, 0);
//   glRasterPos3f(0.0, 0.0, -1.1*length);
//   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'e');
//   glColor3ub(255, 0, 0);
//   glScalef(0.4, 0.4, 0.4);
//   drawaxes();
//   glPopMatrix();
///* don't wait!  
// * start processing buffered OpenGL routines 
// */
//   glFlush ();
//}
//
//void init (void) 
//{
///* select clearing color 	*/
//   glClearColor (0.0, 0.0, 0.0, 0.0);
//
///* initialize viewing values  */
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
//   gluPerspective(45,1,-0.01,1000);
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();
//   gluLookAt(1,1,1,0,0,0,0,0,1);
//}
//
///* 
// * Declare initial window size, position, and display mode
// * (single buffer and RGBA).  Open window with "hello"
// * in its title bar.  Call initialization routines.
// * Register callback function to display graphics.
// * Enter main loop and process events.
// */
//int main(int argc, char** argv)
//{
//   glutInit(&argc, argv);
//   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
//   glutInitWindowSize (250, 250); 
//   glutInitWindowPosition (100, 100);
//   glutCreateWindow ("hello");
//   init ();
//   glutDisplayFunc(display); 
//   glutMainLoop();
//   return 0;   /* ANSI C requires main to return int. */
//}
//


#include <iostream>
using namespace std;
void main()
{
	///////////////////////////////////////////
	double pi = 3.14159;
	const int &ir = pi;
	const double &dr = pi;

	cout << ir << "\t" << dr << endl;


	///////////////////////////////////////////
	int &ir2 = pi;
	double &dr2 = pi;
	cout << ir2 << "\t" << dr2 << endl;

	////////////////////////////////////////////
	int buf_size = 512;
	double a[buf_size];
}
