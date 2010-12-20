/*
 * @(#)$Id: bass_app.cpp [7/11/2010 RenYaFei] dizuo@126.com $ 
 * @(#)基于OpenGL的声音可视化
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
std::vector<float> wave_ampli;
int	g_max_ampli(0);
int g_min_ampli(1000);

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
// scan the peaks
void ScanPeaks(DWORD decoder)
{
	DWORD length = BASS_ChannelGetLength(decoder, BASS_POS_BYTE);
	g_bypePerPixel=BASS_ChannelGetLength(decoder,BASS_POS_BYTE)/WIDTH; // bytes per pixel
	if (g_bypePerPixel<BASS_ChannelSeconds2Bytes(decoder,0.02)) // minimum 20ms per pixel (BASS_ChannelGetLevel scans 20ms)
		g_bypePerPixel=BASS_ChannelSeconds2Bytes(decoder,0.02);

	DWORD cpos=0,peak[2]={0};
	while (true) {
		DWORD level=BASS_ChannelGetLevel(decoder); // scan peaks
		DWORD pos;
		if (peak[0]<LOWORD(level)) peak[0]=LOWORD(level); // set left peak
		if (peak[1]<HIWORD(level)) peak[1]=HIWORD(level); // set right peak
		if (!BASS_ChannelIsActive(decoder))
			pos=-1; // reached the end
		else 
			pos=BASS_ChannelGetPosition(decoder,BASS_POS_BYTE) / g_bypePerPixel;
		if (pos>cpos) {
			DWORD a(0);

			int max_ampli = peak[0]*(HEIGHT/2)/MAX_PEAK;
			int min_ampli = peak[1]*(HEIGHT/2)/MAX_PEAK;
			int avg_ampli = (max_ampli + min_ampli)/2;

			if (avg_ampli>g_max_ampli)
				g_max_ampli = avg_ampli;
			if (avg_ampli<g_min_ampli)
				g_min_ampli = avg_ampli;

			wave_ampli.push_back( avg_ampli );

			if (pos>=WIDTH) 
				break; // gone off end of display
			cpos=pos;
			peak[0]=peak[1]=0;
		}
	}

	BASS_StreamFree(decoder); // free the decoder
}
//////////////////////////////////////////////////////////////////////////
void CALLBACK LoopSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE); // failed, go to start of file instead
}


//////////////////////////////////////////////////////////////////////////
bool PlayFile()
{
	const char* file=file_name.c_str();

	if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,0))
		&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMPS|BASS_MUSIC_POSRESET|BASS_MUSIC_PRESCAN,0))) {
			return FALSE; // Can't load the file
	}

	// repeat playing
	BASS_ChannelSetSync(chan,BASS_SYNC_END|BASS_SYNC_MIXTIME,0,LoopSyncProc,0); // set sync to loop at end

	BASS_ChannelPlay(chan,false);
	return true;
}

void init()
{
	//////////////////////////////////////////////////////////////////////////
	// Init bass
	if(!BASS_Init(-1,44100,0,0,NULL))
	{
		std::cout << ("Can't initialize device");
		return ;
	}

	if(!PlayFile())
	{ // start a file playing
		BASS_Free();
		std::cout << "Cannot play the file" << std::endl;
		return;
	}

	DWORD chan2=BASS_StreamCreateFile(FALSE, file_name.c_str(),0,0,BASS_STREAM_DECODE);
	if (!chan2) 
		chan2=BASS_MusicLoad(FALSE, file_name.c_str(),0,0,BASS_MUSIC_DECODE,0);
	ScanPeaks(chan2);

	for (size_t i(0); i<wave_ampli.size(); i++)
		std::cout << wave_ampli[i] << std::endl;

	cout << "max" << g_max_ampli << endl;
	cout << "min" << g_min_ampli << endl;
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

	QWORD pos = BASS_ChannelGetPosition(chan,BASS_POS_BYTE);
	DWORD wpos = pos / g_bypePerPixel;		//compute the current position

	DWORD peaks_level=BASS_ChannelGetLevel(chan);	// get current peak's level
	DWORD avg_level = ( LOWORD(peaks_level) + HIWORD(peaks_level) )/2;	//compute the average level:0~32768
	DWORD peaks_height = avg_level*(peaks_height_max)/MAX_PEAK;		//compress the level to 0~100

	const float peaks_delta = WIDTH/(float)wave_ampli.size();
	float scale_f = (float)avg_level/(float)MAX_PEAK;	//compress the scale factor(0~1)

	std::cout << scale_f << std::endl;
	std::cout << wpos << std::endl;

	glPushMatrix();
	glTranslatef(0, 0, g_camera_z);

	glPushMatrix();
	{	
		// draw peaks
		glBegin(GL_LINE_STRIP);
			//glVertex3f(beginPos, wave_ampli[0], 0);
			for (int i(0); i<wave_ampli.size(); i++)
				glVertex3f(i*peaks_delta + peaks_beginPos, wave_ampli[i], 0);
		glEnd();
	} glPopMatrix();

	//////////////////////////////////////////////////////////////////////////
	// draw the current line and bounding rect edges
	glPushMatrix();
	{
		// draw current position line.
		glBegin(GL_LINES);
			glVertex3f(peaks_beginPos + wpos, 0, 0);
			glVertex3f(peaks_beginPos + wpos, peaks_height, 0);
		glEnd();
		
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
