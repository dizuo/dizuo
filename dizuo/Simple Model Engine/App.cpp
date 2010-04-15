// App.cpp: implementation of the CApp class.
//
//////////////////////////////////////////////////////////////////////

#include "resource.h"
#include "App.h"
#include <crtdbg.h>

CApp* CApp::m_pAppInstance = NULL;

/////////////////////////////////////////////////////////////////////////////
// Global functions
/////////////////////////////////////////////////////////////////////////////

//
// WinMain()
// The applications entry point.
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_ASSERT(CApp::m_pAppInstance); // Need one CApp instance!
	return CApp::m_pAppInstance->Run(hInstance, nCmdShow, lpCmdLine);
}

//
// WndProc()
// Window procedure for the main window. Simply calls CApp::WindowProc.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return CApp::m_pAppInstance->WindowProc(hWnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApp::CApp():
m_hDC(0),
m_hAccelTable(0),
m_hInstance(0),
m_hWnd(NULL),
m_hRC(NULL),
m_hand_action(HA_ROTATE),
m_capture_x(-1),
m_capture_y(-1),
m_bRenderInWireFrame(false),
m_bUseLighting(true)
{
	_ASSERT(m_pAppInstance == NULL); // Only one CApp instace allowed
	m_pAppInstance = this;
}

CApp::~CApp()
{
}

//////////////////////////////////////////////////////////////////////
// CApp member functions
//////////////////////////////////////////////////////////////////////

void CApp::PreCreateWindow(WNDCLASSEX &wcex, DWORD &dwStyle, DWORD &dwExStyle, int &x, int &y, int &cx, int &cy)
{
	// Window class info
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	HINSTANCE hInstance = GetModuleHandle(NULL);

	//wcex.hInstance = m_hInstance;
	wcex.hInstance = hInstance;

	//wcex.hIcon = LoadIcon(m_hInstance, (LPCTSTR)IDI_SME);
	//wcex.hIconSm = LoadIcon(m_hInstance, (LPCTSTR)IDI_SME);
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_SME);
	wcex.hIconSm = LoadIcon(hInstance, (LPCTSTR)IDI_SME);

	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_OPENGL_MENU);


	//wcex.hIconSm = (HICON)LoadImage(m_hInstance, (LPCTSTR)IDR_MAINFRAME, IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	// Window style
	dwStyle = WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION |WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;

	// Extended window style
	dwExStyle = WS_EX_APPWINDOW;

	// Window position and size
	x = y = cx = cy = CW_USEDEFAULT;
}

//
// Run()
// Starts the message pump
//
int CApp::Run(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine)
{  
	if (!InitInstance(hInstance, nCmdShow, lpCmdLine)) 
		return FALSE;

	//开启时钟
	m_Timer.start();

	MSG msg;
	while(TRUE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
				break;
			if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg)) 
			{		
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	ExitInstance();

	return msg.wParam;
}

//
// InitInstance()
// Called when the application starts. Creates and shows the main window.
//
BOOL CApp::InitInstance(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine)
{
	const char szWindowClass[] = "SME";
	const char szWindowTitle[] = "SME";

	m_hInstance = hInstance;

	DWORD dwStyle, dwExStyle;
	int x, y, cx, cy;

	// Register main window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpszClassName = szWindowClass;

	PreCreateWindow(wcex, dwStyle, dwExStyle, x, y, cx, cy);
	wcex.style|= CS_OWNDC;

	if(!RegisterClassEx(&wcex))
		return FALSE;

	// Load accelerator table
	// m_hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MAINFRAME);

	// Create main window
	m_hWnd = CreateWindowEx(dwExStyle, szWindowClass, szWindowTitle, dwStyle, x, y, cx, cy, 0, 0, hInstance, NULL);

	if (!m_hWnd)
		return FALSE;

	//sgInitKernel();
	//sgC3DObject::AutoTriangulate(true, SG_DELAUNAY_TRIANGULATION);

	OnCreate();

	//设置时钟
	SetTimer(m_hWnd, TIMER1, 100, NULL);

	// Show the main window
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

//
// ExitInstance()
// Called when the application exits.
//
void CApp::ExitInstance()
{
	/*sgFreeKernel();*/
}

//
// OnCommand()
// Handles WM_COMMAND messages
//
BOOL CApp::OnCommand(int nCmdID, int nEvent)
{
	return TRUE;
}

//
// AboutProc()
// Handles messages sent to the About dialog
//
LRESULT CALLBACK CApp::AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;		
		}
		break;
	}
	return FALSE;
}

//
// ShowAboutDialog()
// Shows the About dialog
//
int CApp::ShowAboutDialog()
{
	return DialogBox(m_hInstance, (LPCTSTR)IDD_ABOUTBOX, m_hWnd, (DLGPROC)AboutProc);
}

//
// WindowProc()
// Handles messages sent to the main window
//
LRESULT CApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (message) 
	{
	case WM_CLOSE:
		DestroyWindow(m_hWnd);
		m_hWnd = 0;
		break;

	case WM_COMMAND:
		if(!OnCommand(LOWORD(wParam), HIWORD(wParam)))
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_F3:
			m_bRenderInWireFrame = !m_bRenderInWireFrame;
			break;

		case VK_F4:
			m_bUseLighting = !m_bUseLighting;
			break;
		}
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		OnPaint(hDC);
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		if (wParam==TIMER1)
			SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case WM_DESTROY:
		OnDestroy();
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		OnSize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONDOWN:
		OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(LOWORD(lParam),HIWORD(lParam));
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CApp::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nVersion        = 1;
	pfd.dwFlags         = PFD_DRAW_TO_WINDOW| PFD_SUPPORT_OPENGL| 
		PFD_DRAW_TO_BITMAP| PFD_DOUBLEBUFFER  | PFD_SWAP_EXCHANGE;
	pfd.iPixelType      = PFD_TYPE_RGBA;
	pfd.cColorBits      = 32;
	pfd.cRedBits        = 8;
	pfd.cRedShift       = 16; 
	pfd.cGreenBits      = 8; 
	pfd.cGreenShift     = 8; 
	pfd.cBlueBits       = 8; 
	pfd.cBlueShift      = 0; 
	pfd.cAlphaBits      = 0; 
	pfd.cAlphaShift     = 0; 
	pfd.cAccumBits      = 64;
	pfd.cAccumRedBits   = 16; 
	pfd.cAccumGreenBits = 16; 
	pfd.cAccumBlueBits  = 16; 
	pfd.cAccumAlphaBits = 16; 
	pfd.cDepthBits      = 16; 
	pfd.cStencilBits    = 0; 
	pfd.cAuxBuffers     = 0; 
	pfd.iLayerType      = PFD_MAIN_PLANE;
	pfd.bReserved       = 0;
	pfd.dwLayerMask     = 0; 
	pfd.dwVisibleMask   = 0; 
	pfd.dwDamageMask    = 0; 

	int pixelformat;
	if ( (pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0 ) 
		return FALSE;

	if (SetPixelFormat(hDC, pixelformat, &pfd) == FALSE) 
		return FALSE;

	int n = ::GetPixelFormat(NULL);
	::DescribePixelFormat(hDC, n, sizeof(pfd), &pfd);

	return TRUE;
}

//
// OnCreate()
// Called when the main window has been created.
//
void CApp::OnCreate()
{
	// Set pixel format
	m_hDC = GetDC(m_hWnd);
	if(SetWindowPixelFormat(m_hDC))
	{
		m_hRC = wglCreateContext(m_hDC);
		if(m_hRC)
			wglMakeCurrent (m_hDC, m_hRC);
		else
			return;
	}

	InitScene();

	// Size viewport
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_Width = rc.right-rc.left;
	m_Height = rc.bottom-rc.top;

	OnSize(m_Width, m_Height);
}

//
// OnDestroy()
// Called when the main window is destroyed. 
//
void CApp::OnDestroy()
{
	_ASSERT(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);
	KillScene();
	wglMakeCurrent(0, 0);
	wglDeleteContext(m_hRC);
	ReleaseDC(m_hWnd, m_hDC);
	m_hDC = 0;
}

//
// OnSize()
// Called when the main window is resized. 
//
void CApp::OnSize(int cx, int cy)
{
	if(cx==0 || cy ==0 || m_hWnd==NULL)
		return;

	// Size viewport
	m_Width = cx;
	m_Height = cy;

	//m_Camera.ResetView(cx, cy);
}

void CApp::OnLButtonDown(int pX, int pY)
{
	m_capture_x = pX;
	m_capture_y = pY;

	SetCapture(m_hWnd);
}

void CApp::OnMouseMove(int pX, int pY)
{
	if (m_capture_x<0 || m_capture_y<0)
		return ;

	if( GetCapture() != m_hWnd )
		return;

	float deltaX = static_cast<float>(m_capture_x - pX);
	float deltaY = static_cast<float>(m_capture_y - pY);

	switch (m_hand_action )
	{
	case HA_ROTATE:
		{	
			m_Camera.RotateCamera(deltaX, deltaY);

			SendMessage(m_hWnd, WM_PAINT,0,0);
		}
		break;
	case HA_MOVE:
		{
			m_Camera.PanCamera(deltaX, deltaY);

			SendMessage(m_hWnd, WM_PAINT,0,0);
		}
		break;
	case HA_ZOOM:
		{		
			m_Camera.ZoomCamera(deltaY);

			SendMessage(m_hWnd, WM_PAINT,0,0);
		}
		break;
	default:
		break;
	}

	m_capture_x = pX;
	m_capture_y = pY;
}

void CApp::OnLButtonUp(int pX, int pY)
{
	m_capture_x = m_capture_y = -1;
	ReleaseCapture();
}

//
// OnPaint()
// Handles WM_PAINT messages. Redraws the OpenGL scene.
//
void CApp::OnPaint(HDC hDC)
{
	_ASSERT(hDC == m_hDC);
	wglMakeCurrent(m_hDC,m_hRC);

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Camera.ResetView(m_Width, m_Height);

	// Set the matrix mode to MODELVIEW and load
	// the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set our camera position and save its transformation matrix
	m_Camera.PositionCamera();

	//////////////////////////////////////////////////////////////////////////
	if( m_bRenderInWireFrame == true )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	if( m_bUseLighting == true )
		glEnable( GL_LIGHTING );
	else
		glDisable( GL_LIGHTING );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	glPushMatrix();
	//glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	DrawScene();
	glPopMatrix();
	
	//////////////////////////////////////////////////////////////////////////

	glFlush();
	glFinish();
	SwapBuffers(m_hDC);
}