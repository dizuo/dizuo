// App.h: interface for the CApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(APP_H_INCLUDED)
#define APP_H_INCLUDED

#pragma once

#include "Camera\\CCamera.h"
#include "Timer.h"

typedef enum
{
	HA_NONE,
	HA_MOVE,
	HA_ROTATE,
	HA_ZOOM
} HAND_ACTION;

/////////////////////////////////////////////////////////////////////////////
// CApp
// Application base class

class CApp
{
protected:
	HINSTANCE m_hInstance;// Handle to application instance
	HACCEL m_hAccelTable;	// Accelerator table
	HWND m_hWnd;			// Handle to window

	HDC m_hDC;				// Handle to DC
	HGLRC m_hRC;		  // Handle to RC

	HAND_ACTION m_hand_action;
	
	bool		m_bRenderInWireFrame;
	bool		m_bUseLighting;

	CCamera		m_Camera;
	Timer		m_Timer;

	int			m_capture_x;
	int			m_capture_y;
	int			m_Width;
	int			m_Height;

	virtual void DrawScene() = 0;
	virtual void KillScene() = 0;
	virtual void InitScene() = 0;

	virtual BOOL OnCommand(int nCmdID, int nEvent);	
	void	OnCreate();
	void	OnDestroy();
	void	OnSize(int cx, int cy);

	void	OnLButtonDown(int, int);
	void	OnMouseMove(int, int);
	void	OnLButtonUp(int, int);

	virtual void	OnPaint(HDC hDC);
	virtual void	PreCreateWindow(WNDCLASSEX &wcex, DWORD &dwStyle, DWORD &dwExStyle, int &x, int &y, int &cx, int &cy);
	BOOL			SetWindowPixelFormat(HDC hDC);
	virtual BOOL	InitInstance(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine);
	virtual void	ExitInstance();

	//////////////////////////////////////////////////////////////////////////
	//camera operation
	//////////////////////////////////////////////////////////////////////////

	int ShowAboutDialog();
	static LRESULT CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static CApp* m_pAppInstance;

	int Run(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine);
	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	CApp();
	virtual ~CApp();
};

#endif // !defined(APP_H_INCLUDED)
