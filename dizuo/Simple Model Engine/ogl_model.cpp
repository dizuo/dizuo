//-----------------------------------------------------------------------------
//           Name: ogl_model.cpp
//   Control Keys: Left Mouse Button - Spin the view
//                 F3 - Toggle wire-frame mode
//                 F4 - Toggle lighting
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include "CCylinder.h"
#include "CSphere.h"
#include "CTorus.h"
#include "CLoft.h"
#include "CLathe.h"
#include "CCurve.h"
#include "CExtrude.h"

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "resource.h"

#include <stack>
#include "CCircle.h"

#include "Timer.h"
#include "WaveApplyer.h"
#include "TwistApplyer.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HDC			g_hDC       = NULL;
HGLRC		g_hRC       = NULL;
HWND		g_hWnd      = NULL;
HINSTANCE	g_hInstance = NULL;
GLuint		g_textureID = 0;

bool		g_bUseLighting = true;
bool		g_bRenderInWireFrame = false;
int			g_nPrecision = 32;

float		g_fSpinX = 0.0f;
float		g_fSpinY = 0.0f;

Timer		g_Timer;
//////////////////////////////////////////////////////////////////////////

CSphere		g_Sphere(1.5f, g_nPrecision);
CTorus		g_Torus(2.0f, 0.25f, g_nPrecision);

CCylinder	g_Cylinder(.5, 2.0f, 4, 5);
TwistApplyer*	g_pTwistApplyer = NULL;

CLoft		g_Lofter;
WaveApplyer*	g_pWaveApplyer = NULL;

CLathe		g_Lather;
CExtrude	g_Extruder;
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE g_hInstance,HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND g_hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void loadTexture(void);
void init(void);
void render(void);
void shutDown(void);
void redirectIOToConsole(void);

//
// Menu command processing
// There will be more here later.
//
int MenuCommand( HWND hWnd, int wmId, int wmEvent )
{
	switch (wmId)
	{
	case IDM_EXIT:
		DestroyWindow(hWnd);
		return TRUE;
	}
	return FALSE;
}
//-----------------------------------------------------------------------------
// Name: WinMain
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	redirectIOToConsole();

	WNDCLASSEX winClass;
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
    winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_SME);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_SME);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = MAKEINTRESOURCE(IDC_OPENGL_MENU);;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;
	
	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL,"MY_WINDOWS_CLASS",
						    "OpenGL - Model",
							WS_OVERLAPPEDWINDOW,
					 	    0,0, 640,480, NULL, NULL, g_hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	g_Timer.start();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		    render();
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", g_hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   g_hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
    static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;
    
    switch( msg )
	{
		case WM_COMMAND:
			if ( !MenuCommand( g_hWnd, LOWORD(wParam), HIWORD(wParam) ) )
					return DefWindowProc(g_hWnd, msg, wParam, lParam);
			break;
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

                case VK_F1:
                    if( g_nPrecision > 5 )
                        g_nPrecision -= 2;
                    break;

                case VK_F2:
                    if( g_nPrecision < 30000 )
                        g_nPrecision += 2;
                    break;

                case VK_F3:
                    g_bRenderInWireFrame = !g_bRenderInWireFrame;
                    break;

                case VK_F4:
                    g_bUseLighting = !g_bUseLighting;
                    break;
			}
		}
        break;

        case WM_LBUTTONDOWN:
		{
			ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD (lParam);
            ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD (lParam);
			bMousing = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			bMousing = false;
		}
		break;

		case WM_MOUSEMOVE:
		{
			ptCurrentMousePosit.x = LOWORD (lParam);
			ptCurrentMousePosit.y = HIWORD (lParam);

			if( bMousing )
			{
				g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
				g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
			}
			
			ptLastMousePosit.x = ptCurrentMousePosit.x;
            ptLastMousePosit.y = ptCurrentMousePosit.y;
		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;
		
		default:
		{
			return DefWindowProc( g_hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: loadTexture()
// Desc: 
//-----------------------------------------------------------------------------
void loadTexture(void)	
{
    AUX_RGBImageRec *pTextureImage = auxDIBImageLoad( ".\\earth.bmp" );

    if( pTextureImage != NULL )
    {
        glGenTextures( 1, &g_textureID );

        glBindTexture( GL_TEXTURE_2D, g_textureID );

        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTexImage2D( GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
            GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data );
    }

    if( pTextureImage )
    {
        if( pTextureImage->data )
            free( pTextureImage->data );

        free( pTextureImage );
    }
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
	GLuint PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 16;
    pfd.cDepthBits = 16;
	
	g_hDC = GetDC( g_hWnd );
	PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
	SetPixelFormat( g_hDC, PixelFormat, &pfd );
	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

    loadTexture();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f );

    // Setup lighting
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    float fAmbientColor[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv( GL_LIGHT0, GL_AMBIENT, fAmbientColor );

    float fDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, fDiffuseColor );

    float fSpecularColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_SPECULAR, fSpecularColor );

    float fPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv( GL_LIGHT0, GL_POSITION, fPosition );

    GLfloat ambient_lightModel[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );

	g_Sphere.Setup();
	g_Torus.Setup();
	g_Cylinder.Setup();

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// Loft
	//////////////////////////////////////////////////////////////////////////
	RVec3Array shape;
	CCircle circle1(0.0f, 0.0f, 0.0f, 0.5f, 20, XOY);
	circle1.Setup();
	shape = circle1.GetProfile();
	// the shape must be closed

	RVec3Array path;

	Real pathRadius = 2.0;
	int pathSegMent = 20;
	for (int i=10; i<=pathSegMent; i++)
	{
		float cx = pathRadius * cos( (2*M_PI*i)/pathSegMent );
		float cz = pathRadius * sin( (2*M_PI*i)/pathSegMent );
		path.push_back( RVec3(cx, 0.0f, cz) );
	}
	
	std::stack<RVec3> vecStack;
	for (int i=0; i<pathSegMent/2; i++)
	{
		float cx = 4.0f + pathRadius * cos( (2*M_PI*i)/pathSegMent );
		float cz = pathRadius * sin( (2*M_PI*i)/pathSegMent );
		vecStack.push( RVec3(cx, 0.0f, cz) );
	}

	while( !vecStack.empty() )
	{
		path.push_back( vecStack.top() );
		vecStack.pop();
	}

	g_Lofter.SetShape(shape);
	g_Lofter.SetPath(path);
	g_Lofter.Setup();

	g_pWaveApplyer = new WaveApplyer;
	g_pTwistApplyer = new TwistApplyer;
	g_pTwistApplyer->SetHeight(2.0f);

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// Lathe
	//////////////////////////////////////////////////////////////////////////
	RVec3Array latheProfile;
	//latheProfile.push_back( RVec3(1.0f, 0.0f, 0.0f) );
	//latheProfile.push_back( RVec3(2.0f, 0.0f, 0.0f) );
	//latheProfile.push_back( RVec3(2.0f, 1.0f, 0.0f) );
	//latheProfile.push_back( RVec3(1.0f, 1.0f, 0.0f) );
	latheProfile.push_back( RVec3(1.0f, 0.0f, 0.0f) );
	latheProfile.push_back( RVec3(2.0f, 0.0f, 0.0f) );
	latheProfile.push_back( RVec3(2.2f, 1.0f, 0.0f) );
	latheProfile.push_back( RVec3(2.2f, 2.0f, 0.0f) );
	latheProfile.push_back( RVec3(2.0f, 3.0f, 0.0f) );
	latheProfile.push_back( RVec3(1.0f, 3.0f, 0.0f) );
	latheProfile.push_back( RVec3(1.0f, 0.0f, 0.0f) );

	g_Lather.SetProfile( latheProfile );
	g_Lather.Setup();

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// Extrude
	//////////////////////////////////////////////////////////////////////////
	CCircle xozCircle(0.0f, 0.0f, 0.0f, 1.0f);
	xozCircle.Setup();

	g_Extruder.SetProfile( xozCircle.GetProfile() );
	g_Extruder.Setup();
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )	
{
    glDeleteTextures( 1, &g_textureID );
        
	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;							
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
	// Clear the screen and the depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -10.0f );
    glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX, 0.0f, 1.0f, 0.0f );

    if( g_bRenderInWireFrame == true )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    if( g_bUseLighting == true )
        glEnable( GL_LIGHTING );
    else
        glDisable( GL_LIGHTING );

	{	//draw axis
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(50.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 50.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 50.0);
		glEnd();
	}
    
 	//g_Sphere.Render();
 	//g_Torus.Render();
	//g_Lather.Render();
	g_Extruder.Render();
	
	//twist group test
	//g_pTwistApplyer->SetTime( g_Timer.getElapsedTimeInMilliSec()*0.001f );
	//(*g_pTwistApplyer)(g_Cylinder);
	//g_Cylinder.Render();
	//twist group test

	// wave group
	//(*g_pWaveApplyer)(g_Lofter, g_Timer.getElapsedTime());
	//g_Lofter.Render();
	// wave group

	SwapBuffers( g_hDC );
}

void redirectIOToConsole( void )
{
	// Allocate a console so we can output some useful information.
	AllocConsole();

	// Get the handle for STDOUT's file system.
	HANDLE stdOutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );

	// Redirect STDOUT to the new console by associating STDOUT's file 
	// descriptor with an existing operating-system file handle.
	int hConsoleHandle = _open_osfhandle( (intptr_t)stdOutputHandle, _O_TEXT );
	FILE *pFile = _fdopen( hConsoleHandle, "w" );
	*stdout = *pFile;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// This call ensures that iostream and C run-time library operations occur  
	// in the order that they appear in source code.
	ios::sync_with_stdio();
}