//////////////////////////////////////////////////////////////////////////
// Camera: 
//	setViewport
//	setFov
//	setNearPlane, setFarPlane

// TrackBall
//	setCamera
//	setRenderTarget: height,width

//////////////////////////////////////////////////////////////////////////
#include <vl/Camera.hpp>
#include <vl/TrackballManipulator.hpp>

#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>

int							gWindowWidth = 800;
int							gWindowHeight = 600;

vl::ref<vl::Camera>			gCamera;
vl::ref<vl::TrackballManipulator>	gTrackManipulator;

void init()
{
	//////////////////////////////////////////////////////////////////////////
	// 1. Setup camera and trackball manipulator
	//////////////////////////////////////////////////////////////////////////
	vl::vec3 eye    = vl::vec3(0,20,35); // camera position
	vl::vec3 center = vl::vec3(0,0,0);   // point the camera is looking at
	vl::vec3 up     = vl::vec3(0,1,0);   // up direction
	vl::mat4 view_mat = vl::mat4::lookAt(eye, center, up).inverse();

	gCamera = new vl::Camera;
	gCamera->setViewMatrix( view_mat );
	gCamera->setViewport( 
		new vl::Viewport(0, 0, gWindowWidth, gWindowHeight) );

	gTrackManipulator = new vl::TrackballManipulator;
	gTrackManipulator->setCamera( gCamera.get() );
	//////////////////////////////////////////////////////////////////////////

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display (void)
{
	//////////////////////////////////////////////////////////////////////////
	// 2.apply the camera
	//////////////////////////////////////////////////////////////////////////
	gTrackManipulator->camera()->activate();
	//////////////////////////////////////////////////////////////////////////

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{	
		glutSolidCube(2.0f);
	}glPopMatrix();

	glutSwapBuffers();
}



void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	//glMatrixMode (GL_PROJECTION);
	//glLoadIdentity ();
	//gluPerspective(60.0, (float)w/h, 1.0, 20.0);	

	//glMatrixMode (GL_MODELVIEW);
	//glLoadIdentity ();
	//glTranslatef(0.0, 0.0, -10.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		exit(1);
		break;
	case ' ':
		//////////////////////////////////////////////////////////////////////////
		// 4. Dispatch the keyboard event
		//////////////////////////////////////////////////////////////////////////
		gTrackManipulator->adjustView( vl::AABB(vl::vec3(0,0,0), 2.0f),
									vl::vec3(0,1,1),
									vl::vec3(0,1,0) );
		//////////////////////////////////////////////////////////////////////////
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y)
{
	//////////////////////////////////////////////////////////////////////////
	// 4. Dispatch the mouse down event.
	//////////////////////////////////////////////////////////////////////////
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		gTrackManipulator->mouseDownEvent(vl::LeftButton, x, y);

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		gTrackManipulator->mouseDownEvent(vl::RightButton, x, y);

	if(button == GLUT_MIDDLE_BUTTON && state ==GLUT_DOWN )
		gTrackManipulator->mouseDownEvent(vl::MiddleButton, x, y);


	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		gTrackManipulator->mouseUpEvent(vl::LeftButton, x, y);

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		gTrackManipulator->mouseUpEvent(vl::RightButton, x, y);

	if(button == GLUT_MIDDLE_BUTTON && state ==GLUT_UP )
		gTrackManipulator->mouseUpEvent(vl::MiddleButton, x, y);
	//////////////////////////////////////////////////////////////////////////

	glutPostRedisplay();	
}

void motion(int x, int y)
{	
	//////////////////////////////////////////////////////////////////////////
	// 5. Dispatch the mouse motion event.
	//////////////////////////////////////////////////////////////////////////
	gTrackManipulator->mouseMoveEvent(x, y);
	//////////////////////////////////////////////////////////////////////////

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (gWindowWidth, gWindowHeight);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	init();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop ();

	return 0;
}





