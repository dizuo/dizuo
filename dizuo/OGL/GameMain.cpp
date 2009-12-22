#include	<iomanip>
#include	"Scene.h"


//////////////////////////////////////////////////////////////////////////
// Global Variables
Scene	mySceneHandle;
GLint	g_iWidth = 1024;
GLint	g_iHeight = 768;
		// for the game mode:
char	g_sCurrentMode[80];
		// this string keeps the last good setting 
char	g_sGameModeString[40] = "640x480";
Font	*g_pFont = NULL;				

//////////////////////////////////////////////////////////////////////////
// Functions:
void GameInit(void);
void MenuInit(void);
void Display(void);
void Reshape(int x, int y);

void MenuHandle(int);
void OnMouseMotionHandle(int x, int y);
void OnKeyDownHandle(unsigned char key, int x, int y);
void OnMouseHandle(int button, int state, int x, int y);
void OnSpecialHandle(int key, int x, int y);

void SetOrthoProjection();
void ResetPerspectiveProjection();

void Run();
//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(800,600);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Game");
	
	Run();

	glutMainLoop();

	delete g_pFont;
	g_pFont = NULL;
	return 0;      
}

void Run()
{
	GameInit();
	MenuInit();

	mySceneHandle.SetWindow(g_iWidth, g_iHeight);

	glutDisplayFunc( Display );
	glutReshapeFunc( Reshape );
	glutKeyboardFunc( OnKeyDownHandle );
	//不对特殊键响应
	//glutSpecialFunc( OnSpecialHandle );
	glutMouseFunc( OnMouseHandle );
	glutMotionFunc( OnMouseMotionHandle );
}

void GameInit()
{
	glEnable( GL_SMOOTH );
	glEnable( GL_NORMALIZE );
	//glEnable( GL_NORMALIZE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );

	glDisable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	float lightpos[]	= { 10.0, 10.0, 100.0 };
	float lightcolor[]	= { 1.0, 1.0, 1.0, 1.0 };

	glLightfv( GL_LIGHT0, GL_POSITION, lightpos );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightcolor );
	glLightfv( GL_LIGHT0, GL_SPECULAR, lightcolor );

	// Scene Init...
	if( mySceneHandle.SceneInit()==false )
	{
		printf("The Scene Init failed. exit....\n");
		exit(1);
	}
	
	g_pFont = new Font;
	g_pFont->SetFontType("Times Roman 24");
	//坐标系:原点左上角，y轴向上
	g_pFont->AddFontElem(30, 35, 0.0,"GLUT Tutorial @ 3D Tech"); 
	g_pFont->AddFontElem(30, 75, 0.0,"F1 - Game Mode 1024x768 32 bits");
	g_pFont->AddFontElem(30, 115, 0.0, "F2 - Game Mode 1280x1024 32 bits");
	g_pFont->AddFontElem(30, 155, 0.0, "F3 - Game Mode 1600x1200 32 bits");
	g_pFont->AddFontElem(30, 195, 0.0, "F4 - Window Mode");
}

void MenuInit()
{
	glutCreateMenu(MenuHandle);
	glutAddMenuEntry("Camera Manipulator", Scene::SCENE);
	glutAddMenuEntry("Examine The Role", Scene::ROLE);
	glutAddMenuEntry("Examine The House", Scene::HOUSE);
	glutAddMenuEntry("Examine The Teapot", Scene::TEAPOT);
	glutAddMenuEntry("Examine The Gun", Scene::GUN);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void MenuHandle(int value)
{
	switch(value)
	{
	case 0:
		mySceneHandle.SetApplyMode(Scene::SCENE);
		break;
	case 1:
		mySceneHandle.SetApplyMode(Scene::ROLE);
		break;
	case 2:
		mySceneHandle.SetApplyMode(Scene::HOUSE);
		break;
	case 3:
		mySceneHandle.SetApplyMode(Scene::TEAPOT);
		break;
	case 4:
		mySceneHandle.SetApplyMode(Scene::GUN);
		break;
	default:
		break;
	}
}


void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  

	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluPerspective(80.0,(GLdouble)x/(GLdouble)y,0.001, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0,0,x,y);  
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable( GL_TEXTURE_2D );

	SetOrthoProjection();
	glPushMatrix();
	g_pFont->Draw();
	glPopMatrix();
	ResetPerspectiveProjection();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glLoadIdentity();
		// Scene Display...
		mySceneHandle.SceneDisplay();
	}
	glPopMatrix();	

	//glFlush();  
	glutSwapBuffers();

	glutPostRedisplay();
}

void SetOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();				//对应的glPopMatrix()在ResetPerspectiveProjection()函数中
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, g_iWidth, 0, g_iHeight);
	glScalef(1, -1, 1);				// 转置y坐标，向下为正
	glTranslatef(0, -g_iHeight, 0);	//将坐标原点移到左上角
									//请注意：
									//这里字体的坐标系和Scene中Font所用坐标系不一样，
	glMatrixMode(GL_MODELVIEW);
}

void ResetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}
void OnMouseMotionHandle(int x, int y)
{
	mySceneHandle.OnMotionHandle(x, y);
	glutPostRedisplay();
}
void OnKeyDownHandle(unsigned char key, int x, int y)
{
	mySceneHandle.KeyboardHandle();
}

void OnMouseHandle(int button, int state, int x, int y) 
{ 
	mySceneHandle.OnMouseHandle(button, state, x, y);
	glutPostRedisplay();
} 
void OnSpecialHandle(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_F1:  
 			// define resolution, color depth
 			glutGameModeString("1024x768:32");
 			// enter full screen
 			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
 				glutEnterGameMode();
 				sprintf(g_sGameModeString,"1024x768:32");
 				// register callbacks again 
 				// and init OpenGL context
 				Run();
 			}
 			else
 				glutGameModeString(g_sGameModeString);
/*			全屏
			g_iWidth = (int)glutGet(GLUT_SCREEN_WIDTH);
			g_iHeight= (int)glutGet(GLUT_SCREEN_HEIGHT);
			Run();*/

			break;
		case GLUT_KEY_F2:
			// define resolution, color depth
			glutGameModeString("1280x1024:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(g_sGameModeString,"1280x1024:32");
				// register callbacks again 
				// and init OpenGL context
				g_iWidth = 1280, g_iHeight = 1024;
				Run();
			}
			else
				glutGameModeString(g_sGameModeString);
			break;
		case GLUT_KEY_F3:  
			// define resolution, color depth
			glutGameModeString("1600x1200");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(g_sGameModeString,"1600x1200");
				// register callbacks again 
				// and init OpenGL context
				g_iWidth = 1600, g_iHeight = 1200;
				Run();
			}
			else
				glutGameModeString(g_sGameModeString);
			break;
		case GLUT_KEY_F4: 
			// return to default window
			g_iWidth = 1024; g_iHeight = 768;
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
				glutLeaveGameMode();
			break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf(g_sCurrentMode,"Current Mode: Window");
	else
		sprintf(g_sCurrentMode,
		"Current Mode: Game Mode %dx%d at %d hertz, %d bpp",
		glutGameModeGet(GLUT_GAME_MODE_WIDTH),
		glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
		glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
		glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
}

//////////////////////////////////////////////////////////////////////////
// 增加game模式，失败...
// void OnSpecialHandle(int key, int x, int y);
// void SetOrthoProjection();
// void ResetPerspectiveProjection();
// void Run();