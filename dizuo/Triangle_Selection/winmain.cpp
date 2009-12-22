//
// A Mesh Viewer / Triangle Selector demonstration program
//
// By Jonathan Kreuzer
//
// All of the windows specific stuff is in this file, plus the RenderFrame function
// 
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <string>
#include "resource.h"

#include "vec3.h"
#include "matrix.h"
#include "renderInterface.h"
#include "mesh.h"
#include "viewport.h"

// Global Classes
CView g_View;

CRenderer *g_pRenderer = NULL;
CMesh g_Mesh;
CSelection g_Select;
		
// Functions
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void RenderFrame( class CRenderer *pRender );

// Main
int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MyRegisterClass(hInstance);
	
	HWND hWnd = CreateWindow( "testappclass", "Sample App: stage 2 ( + group selection )", WS_OVERLAPPEDWINDOW,
				 CW_USEDEFAULT, 0, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE; 

	// Initialize Renderer
	CRenderer *pRender = new CRenderer;
	g_pRenderer = pRender;
	pRender->InitRenderWindow( hWnd, true, true );
	if ( !pRender->IsInitialized() ) return 0;

	// Show the Window
	MoveWindow( hWnd, 20, 20, 800, 600, TRUE );
    ShowWindow( hWnd, nCmdShow );
    UpdateWindow(hWnd);
	
	// Load the Mesh	
	g_Mesh.Load( "meshhead.dat" );
	
	// Main loop
	MSG msg;
	bool bDone = false;
	
	while ( !bDone ) 
	{
		// Render the Frame
		RenderFrame( pRender );	
		
		// Check for and process windows messages each frame
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE ) )
		{		
			if ( msg.message == WM_QUIT ) {
				bDone = true;
			} else {	
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	// Exit
	pRender->Shutdown( );
	return (int) msg.wParam;
}
//
// 
//
void RenderFrame( CRenderer *pRender )
	{
	pRender->ClearBuffer( );
		
	// Rotate the Viewport
	CMatrix ModelView = g_View.GetMatrix();
	pRender->SetMatrix( ModelView );
	
	g_Mesh.Render( pRender );
	g_Select.Render( pRender );	
	
	// This is so selection, which uses the gluUnproject, will have the proper matrices
	pRender->SetMatrix( g_View.GetMatrix() );

	// Show the back buffer
	pRender->BufferSwap( );		
	}
	
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

//
// 
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	POINT Point;
	static bool bRight = false, bMiddle = false;
	static POINT StartPoint = {0,0};
	
	switch (message)
	{
	case WM_COMMAND:
		if ( !MenuCommand( hWnd, LOWORD(wParam), HIWORD(wParam) ) )
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
		
	case WM_ERASEBKGND:
		// This window is fully drawn by OpenGL, so don't allow any standard window erasing
		// ( ps. I think BKGND is on of the weirdest and hardest to remember abbreviations I've seen )
		return false;
			
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (g_pRenderer && g_pRenderer->IsInitialized() ) 
			{
			RenderFrame( g_pRenderer );
			}
		EndPaint(hWnd, &ps);
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN: 		
	    SetCapture( hWnd ); // So user can drag mouse beyond the window edge without messing things up
	    
		g_Mesh.MarkBackfacing( g_View.GetCamPos() );
		// Send mouse location in window coordinates
        g_Select.ButtonDown( LOWORD(lParam), HIWORD(lParam), &g_Mesh );
		break;
		
	case WM_LBUTTONUP:
		ReleaseCapture( );
		g_Mesh.MarkBackfacing( g_View.GetCamPos() );
		g_Select.ButtonUp( &g_Mesh );
		break;
				
	case WM_RBUTTONDOWN:
		SetCapture( hWnd );
		// store mouse location for the full screen coordinates
		GetCursorPos(&StartPoint); 
		bRight = true;	
		break;
		
	case WM_RBUTTONUP:
		ReleaseCapture( );
		bRight = false;
		break;
	
	case WM_MBUTTONDOWN:
		SetCapture( hWnd );
		GetCursorPos(&StartPoint);
		bMiddle = true;
		break;
		
	case WM_MBUTTONUP:
		ReleaseCapture( );
		bMiddle = false;
		break;
		
	case WM_SIZE:

		g_pRenderer->SetSize( LOWORD(lParam), HIWORD(lParam) );
		g_Select.SetWindowHeight( HIWORD(lParam) ); // keep track of the window height so we can flip the mouse y	
		break;
	
	case WM_MOUSEMOVE:
	
		g_Select.MouseMove( LOWORD(lParam), HIWORD(lParam) );
		GetCursorPos(&Point);
		
		if (bRight) {			
			g_View.RotateByDeg( (Point.x - StartPoint.x) / 2.0f, (Point.y - StartPoint.y) / 2.0f );
			StartPoint = Point;
			}
			
		if ( bMiddle ) {
			// Shift zooms, otherwise pan
			if ( GetKeyState( VK_SHIFT ) < 0 ) 
				g_View.DragView( CVec3( 0, 0, -float(Point.y - StartPoint.y ) * 2.0f ) );	
			else
				g_View.DragView( CVec3( float(Point.x - StartPoint.x), -float(Point.y - StartPoint.y ), 0 ) );
				
			StartPoint = Point;
			}
		break;
				
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	return 0;
}
//
//  
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LINEMESHTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LINEMESHTEST);
	wcex.lpszClassName	= "testappclass";
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}	