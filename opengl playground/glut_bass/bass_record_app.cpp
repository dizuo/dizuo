/*
 * @(#)$Id: bass_app.cpp [09/11/2010 RenYaFei] dizuo@126.com $ 
 * @(#)基于Bass的录音可视化
 * Author: Dizuo.Hangzhou.
 * All Rights Reserved.
 * bass download URL: http://www.un4seen.com/
 */

#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include "bass.h"

#pragma comment( lib, "bass.lib")
using namespace std;

//////////////////////////////////////////////////////////////////////////
float	g_camera_z = 0;

bool g_stop(false);
std::string file_name("jazz_drum.mp3");
//std::string file_name("only for you.mp3");

DWORD	chan;					//the file handle
DWORD	g_bypePerPixel;			//

const int MAX_PEAK = 32768;	// peak max amplitude
const int WIDTH = 600;		// display width
const int HEIGHT = 201;		// height (odd number for centre line)

//////////////////////////////////////////////////////////////////////////
void CALLBACK LoopSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE); // failed, go to start of file instead
}


//////////////////////////////////////////////////////////////////////////
// Recording callback - not doing anything with the data
BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE; // continue recording
}

void init()
{
	//////////////////////////////////////////////////////////////////////////
	// initialize BASS recording (default device)
	if (!BASS_RecordInit(-1)) {
		std::cout << ("Can't initialize device") << std::endl;
		return;
	}
	// start recording (44100hz mono 16-bit)
	if (!(chan=BASS_RecordStart(44100,1,0,&DuffRecording,0)))
	{
		std::cout << ("Can't start recording") << std::endl;
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GLfloat mat_diffuse[] = { 1.0, 1.0, 0.0 };
	GLfloat mat_specular[] = {0.8, 0.8, 0.0, 1.0};
	GLfloat mat_shininess[] = { 300. };
	GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 0.0 };
	GLfloat light_ambient[] = {0.7, 0.2, 0.2, 1.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
// The sound wave will be showed in such rect:
//	(-w/2,h/2)				(w/2,h/2)
//
//	(-w/2,	0)				(w/2,  0)
//////////////////////////////////////////////////////////////////////////
void display (void)
{
	//////////////////////////////////////////////////////////////////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(.0, .0, .0);

	//////////////////////////////////////////////////////////////////////////
	const float torus_offset = 50.0f;
	//origin, rightup, leftup, leftdown, rightdown
	const int offset_list[10] = {0,0, 1,1, -1,1, -1,-1, 1,-1};
	const float peaks_beginPos = -WIDTH/2.0f;
	const float peaks_height_max = HEIGHT/2.0f;

	DWORD peaks_level=BASS_ChannelGetLevel(chan);	// get current peak's level
	DWORD avg_level = ( LOWORD(peaks_level) + HIWORD(peaks_level) )/2;	//compute the average level:0~32768
	DWORD peaks_height = avg_level*(peaks_height_max)/MAX_PEAK;		//compress the level to 0~100

	float scale_f = (float)avg_level/(float)MAX_PEAK;	//compress the scale factor(0~1)

	std::cout << scale_f << std::endl;

	glPushMatrix();
	glTranslatef(0, 0, g_camera_z);

	//////////////////////////////////////////////////////////////////////////
	// draw the current line and bounding rect edges
	glPushMatrix();
	{		
		glBegin(GL_LINES);
			glVertex3f(peaks_beginPos, 0, 0);
			glVertex3f(peaks_beginPos, peaks_height_max, 0);

			glVertex3f(-peaks_beginPos, 0, 0);
			glVertex3f(-peaks_beginPos, peaks_height_max, 0);

			glVertex3f(peaks_beginPos, 0, 0);
			glVertex3f(-peaks_beginPos, 0, 0);

			glVertex3f(peaks_beginPos, peaks_height_max, 0);
			glVertex3f(-peaks_beginPos, peaks_height_max, 0);
		glEnd();
	} glPopMatrix();

	//////////////////////////////////////////////////////////////////////////
	// draw five torus
	for (int i(0); i<5; i++)
	{
		glPushMatrix();
		{
			glTranslatef(torus_offset * offset_list[2*i],
						 torus_offset * offset_list[2*i+1], 
						 0.0f);

			if (!g_stop)
				glScalef(scale_f, scale_f, scale_f);
			
			glutSolidTorus(5, 30, 100, 100);
		} glPopMatrix();
	}

	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();

	Sleep(20);
}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (float)w/h, 1.0, 500.0);	
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef(0.0, 0.0, -400.0);
}


void keyboard(unsigned char key, int x, int y)
{
	if (key == 's')
		g_stop = !g_stop;

	if (key == 'e')
		g_camera_z += 1;
	if (key == 'd')
		g_camera_z -= 1;
}

void mouse(int button, int state, int x, int y)
{
}

int main(int argc, char* argv[])
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (1027, 768);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	init();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop ();

	return 0;
}
