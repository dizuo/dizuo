#include "MainWindow.h"
#include "Exception.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameFont.h"

using namespace std;


#define WINDOW_CLASSNAME    "TetroGL"  // Window class name

CMainWindow::CMainWindow(int iWidth, int iHeight, bool bFullScreen) 
  :  m_hWindow(NULL), m_hDeviceContext(NULL), m_hGLContext(NULL), 
     m_bFullScreen(bFullScreen)
{
	RegisterWindowClass();

	RECT WindowRect;
	WindowRect.top = WindowRect.left = 0;
	WindowRect.right = iWidth;
	WindowRect.bottom = iHeight;

	// Window Extended Style
	DWORD dwExStyle = 0;	
	// Windows Style
	DWORD dwStyle = 0;		

	if (m_bFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= iWidth;			
		dmScreenSettings.dmPelsHeight	= iHeight;		
		dmScreenSettings.dmBitsPerPel	= 32;		
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Change the display settings to fullscreen. On error, throw 
		// an exception.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)
				!= DISP_CHANGE_SUCCESSFUL)
		{
			throw CException("Unable to swith to fullscreen mode");
		}

		dwExStyle = WS_EX_APPWINDOW;	
		dwStyle = WS_POPUP;		
		// In fullscreen mode, we hide the cursor.
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;	
	}

	// Adjust the window to the true requested size
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		
	// Now create the main window
    m_hWindow = CreateWindowEx(dwExStyle,TEXT(WINDOW_CLASSNAME), 
							 TEXT("TetroGL"), 
							 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
							 0, 0, WindowRect.right-WindowRect.left, 
							 WindowRect.bottom-WindowRect.top, 
							 NULL, NULL, 
							 GetModuleHandle(NULL), 
							 this);
	if (m_hWindow==NULL)
		throw CException("Cannot create the main window");

	CreateContext();
	InitGL();
	ShowWindow(m_hWindow,SW_SHOW);
	// Call OnSize manually because in fullscreen mode it will be 
	// called only when the window is created (which is too early
	// because OpenGL is not initialized yet).
	OnSize(iWidth,iHeight);

	// Initialize the device context for the fonts:
	CGameFont::SetDeviceContext(m_hDeviceContext);

	m_pStateManager = new CStateManager;
	m_pStateManager->ChangeState(CMenuState::GetInstance(m_pStateManager));
}

CMainWindow::~CMainWindow()
{
	if (m_bFullScreen)
	{
		// Remove the full screen setting
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);	
	}

	if (m_hGLContext)
	{
		// Make the rendering context not current
		wglMakeCurrent(NULL,NULL);
		// Delete the OpenGL rendering context
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	if (m_hDeviceContext)
	{
		// Release the device context
		ReleaseDC(m_hWindow,m_hDeviceContext);
		m_hDeviceContext = NULL;
	}

	// Finally, destroy our main window and unregister the
	// window class.
    DestroyWindow(m_hWindow);
    UnregisterClass(TEXT(WINDOW_CLASSNAME), GetModuleHandle(NULL));
}

LRESULT CMainWindow::OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_NCCREATE)
	{
        // Get the creation parameters.
		CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);

        // Set as the "user data" parameter of the window
        SetWindowLongPtr(Handle, GWLP_USERDATA, 
						 reinterpret_cast<long>(pCreateStruct->lpCreateParams));
	}

    // Get the CMainWindow instance corresponding to the window handle
    CMainWindow* pWindow = reinterpret_cast<CMainWindow*>
		(GetWindowLongPtr(Handle, GWLP_USERDATA));
	if (pWindow)
		pWindow->ProcessEvent(Message,wParam,lParam);

    return DefWindowProc(Handle, Message, wParam, lParam);
}

void CMainWindow::ProcessEvent(UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
	// Quit when we close the main window
    case WM_CLOSE :
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		OnSize(LOWORD(lParam),HIWORD(lParam));
		break;
	// Keydown event
	case WM_KEYDOWN :
		m_pStateManager->OnKeyDown(wParam);
		break;
	// Keyup event
	case WM_KEYUP :
		m_pStateManager->OnKeyUp(wParam);
		break;
	// Character event
	case WM_CHAR:
		m_pStateManager->OnChar(wParam);
		break;
    }
}

void CMainWindow::RegisterWindowClass()
{
    WNDCLASS WindowClass;
    WindowClass.style         = 0;
    WindowClass.lpfnWndProc   = &CMainWindow::OnEvent;
    WindowClass.cbClsExtra    = 0;
    WindowClass.cbWndExtra    = 0;
    WindowClass.hInstance     = GetModuleHandle(NULL);
    WindowClass.hIcon         = NULL;
    WindowClass.hCursor       = 0;
    WindowClass.hbrBackground = 0;
    WindowClass.lpszMenuName  = NULL;
    WindowClass.lpszClassName = WINDOW_CLASSNAME;

    RegisterClass(&WindowClass);
}

void CMainWindow::CreateContext()
{
	// Describes the pixel format of the drawing surface
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;	// Version Number
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |	// Draws to a window
				  PFD_SUPPORT_OPENGL |	// The format must support OpenGL
				  PFD_DOUBLEBUFFER;		// Support for double buffering
	pfd.iPixelType = PFD_TYPE_RGBA;		// Uses an RGBA pixel format
	pfd.cColorBits = 32;				// 32 bits colors

	m_hDeviceContext = GetDC(m_hWindow);
	if (!m_hDeviceContext)	
		throw CException("Unable to create rendering context");

	int PixelFormat;
	// Do Windows find a matching pixel format ?
	PixelFormat = ChoosePixelFormat(m_hDeviceContext,&pfd);
	if (!PixelFormat)				
		throw CException("Unable to create rendering context");
	// Set the new pixel format
	if(!SetPixelFormat(m_hDeviceContext,PixelFormat,&pfd))			
		throw CException("Unable to create rendering context");
	// Create the OpenGL rendering context
	m_hGLContext = wglCreateContext(m_hDeviceContext);
	if (!m_hGLContext)	
		throw CException("Unable to create rendering context");
	// Activate the rendering context
	if(!wglMakeCurrent(m_hDeviceContext,m_hGLContext))
		throw CException("Unable to create rendering context");						
}

void CMainWindow::InitGL()
{
	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	// Choose a smooth shading model
    glShadeModel(GL_FLAT);
	// Set the clear color to black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable blending
	glEnable(GL_BLEND);
}

void CMainWindow::OnSize(GLsizei width, GLsizei height)
{
	// Sets the size of the OpenGL viewport
    glViewport(0,0,width,height);

	// Select the projection stack and apply 
	// an orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,width,height,0.0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
}

void CMainWindow::Update(DWORD dwCurrentTime)
{
	m_pStateManager->Update(dwCurrentTime);
}

void CMainWindow::Draw()
{
	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pStateManager->Draw();

	// Swap the buffers
	SwapBuffers(m_hDeviceContext);
}

