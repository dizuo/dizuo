///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// main driver
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-09-15
// UPDATED: 2008-10-07
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN             // exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "Window.h"
#include "DialogWindow.h"
#include "ControllerMain.h"
#include "ControllerGL.h"
#include "ControllerFormGL.h"
#include "ModelGL.h"
#include "ViewGL.h"
#include "ViewFormGL.h"
#include "resource.h"
#include "Log.h"


// function declarations
int mainMessageLoop(HACCEL hAccelTable=0);





///////////////////////////////////////////////////////////////////////////////
// main function of a windows application
///////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdArgs, int cmdShow)
{
    // register UpDown control from comctl32.dll brfore creating windows
    INITCOMMONCONTROLSEX commonCtrls;
    commonCtrls.dwSize = sizeof(commonCtrls);
    commonCtrls.dwICC = ICC_UPDOWN_CLASS;
    ::InitCommonControlsEx(&commonCtrls);

    // create model and view components for controllers
    ModelGL modelGL;
    Win::ViewGL viewGL;
    Win::ViewFormGL viewFormGL(&modelGL);

    // create main window
    Win::ControllerMain mainCtrl;
    Win::Window mainWin(hInst, L"OpenGL Projection Matrix", 0, &mainCtrl);
    mainWin.setMenuName(MAKEINTRESOURCE(IDR_MENU_MAIN));
    mainWin.setWindowStyleEx(WS_EX_WINDOWEDGE);
    if(mainWin.create())
        Win::log("Main window is created.");
    else
        Win::log("[ERROR] Failed to create main window.");

    // create OpenGL rendering window as a child
    Win::ControllerGL glCtrl(&modelGL, &viewGL);
    Win::Window glWin(hInst, L"WindowGL", mainWin.getHandle(), &glCtrl);
    glWin.setClassStyle(CS_OWNDC);
    glWin.setWindowStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    glWin.setWidth(300);
    glWin.setHeight(600);
    if(glWin.create())
        Win::log("OpenGL child window is created.");
    else
        Win::log("[ERROR] Failed to create OpenGL window.");

    // create a child dialog box contains controls
    Win::ControllerFormGL formCtrl(&modelGL, &viewFormGL);
    Win::DialogWindow glDialog(hInst, IDD_FORMVIEW, mainWin.getHandle(), &formCtrl);
    glDialog.setWidth(300);
    glDialog.setHeight(600);
    if(glDialog.create())
        Win::log("OpenGL form dialog is created.");
    else
        Win::log("[ERROR] Failed to create OpenGL form dialog.");

    // place windows in the right position ================
    const int GL_WIDTH = 300;
    const int GL_HEIGHT = 600;
    const int DIALOG_WIDTH = 365;
    int winWidth;
    int winHeight;

    RECT rect = {0, 0, GL_WIDTH, GL_HEIGHT};                            // exact client size for OpenGL display, 300x600
    DWORD style = ::GetWindowLong(glWin.getHandle(), GWL_STYLE);        // get current window style
    DWORD styleEx = ::GetWindowLong(glWin.getHandle(), GWL_EXSTYLE);    // get current extended window style

    // adjust window size of glWin because camera display should have 300x600 client area
    // It modifies rect struct.
    ::AdjustWindowRectEx(&rect, style, FALSE, styleEx);
    winWidth = rect.right - rect.left;
    winHeight = rect.bottom - rect.top;

    // set glWin first with adjusted window size
    ::SetWindowPos(glWin.getHandle(), 0, 0, 0, winWidth, winHeight, SWP_NOZORDER);

    // set glDialog
    ::SetWindowPos(glDialog.getHandle(), 0, winWidth, 0, DIALOG_WIDTH, winHeight, SWP_NOZORDER);

    // compute window width and height
    winWidth = (rect.right - rect.left) + DIALOG_WIDTH;
    winHeight = rect.bottom - rect.top;

    // set mainWin
    rect.left = 0;
    rect.right = winWidth;
    rect.top = 0;
    rect.bottom = winHeight;
    style = ::GetWindowLong(mainWin.getHandle(), GWL_STYLE);
    styleEx = ::GetWindowLong(mainWin.getHandle(), GWL_EXSTYLE);
    ::AdjustWindowRectEx(&rect, style, TRUE, styleEx);
    ::SetWindowPos(mainWin.getHandle(), 0, 0, 0, (rect.right-rect.left), (rect.bottom-rect.top), SWP_NOZORDER);

    // show all windows
    glWin.show();
    glDialog.show();
    mainWin.show();


    // main message loop //////////////////////////////////////////////////////
    int exitCode;
    HACCEL hAccelTable = 0;
    //hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(ID_ACCEL));
    exitCode = mainMessageLoop(hAccelTable);

    Win::log("Application is terminated.");
    return exitCode;
}



///////////////////////////////////////////////////////////////////////////////
// main message loop
///////////////////////////////////////////////////////////////////////////////
int mainMessageLoop(HACCEL hAccelTable)
{
    HWND activeHandle;
    MSG msg;

    while(::GetMessage(&msg, 0, 0, 0) > 0)  // loop until WM_QUIT(0) received
    {
        // determine the activated window is dialog box
        // skip if messages are for the dialog windows
        activeHandle = GetActiveWindow();
        if(::GetWindowLongPtr(activeHandle, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) // WS_EX_CONTROLPARENT is automatically added by CreateDialogBox()
        {
            if(::IsDialogMessage(activeHandle, &msg))
                continue;   // message handled, back to while-loop
        }

        // now, handle window messages
        if(!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;                 // return nExitCode of PostQuitMessage()
}
