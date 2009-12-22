#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <Windows.h>
#include "GL/gl.h"
#include "StateManager.h"

// The main window class. It wraps the HANDLE of the window and initializes the 
// openGL rendering context. It is also in charge of processing the different
// event messages.
class CMainWindow
{
public:
	CMainWindow(int iWidth, int iHeight, bool bFullScreen);
	~CMainWindow();

	// Called by the application class to update the game logic
	void Update(DWORD dwCurrentTime);
	// Called by the application class when the window need to be redrawn.
	void Draw();

private:
	// Register the window class with the correct window procedure (OnEvent)
	void RegisterWindowClass();
	// Create the rendering context used by OpenGL
	void CreateContext();
	// Initialize openGL
	void InitGL();

	// Called when a WM_SIZE message is received
	void OnSize(GLsizei width, GLsizei height);

	// Static function which will be the window procedure callback
    static LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
	// Processes the messages that were received in OnEvent.
	void ProcessEvent(UINT Message, WPARAM wParam, LPARAM lParam);

	// The window handle
	HWND	m_hWindow;
	// The window device context
    HDC     m_hDeviceContext;
	// The openGL context.
    HGLRC   m_hGLContext;    

	// Specifies if the window is fullscreen.
	bool m_bFullScreen;

	// The state manager
	CStateManager* m_pStateManager;
};

#endif  // _MAINWINDOW_H_