///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2008-10-06
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "ControllerMain.h"
#include "DialogWindow.h"
#include "resource.h"

using namespace Win;

BOOL CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);


int ControllerMain::command(int id, int cmd, LPARAM msg)
{
    switch(id)
    {
    case ID_FILE_EXIT:
        ::PostMessage(handle, WM_CLOSE, 0, 0);
        break;

    case ID_HELP_ABOUT:
        // create a modal dialog box, and destroy it in aboutDialogProcedure().
        ::DialogBox((HINSTANCE)::GetWindowLongPtr(handle, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUT), handle, aboutDialogProcedure);
        break;
    }

    return 0;
}



int ControllerMain::close()
{
    // close all child windows first
    ::EnumChildWindows(handle, (WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);

    ::DestroyWindow(handle);                    // close itself
    return 0;
}



int ControllerMain::create()
{
    return 0;
}



int ControllerMain::size(int w, int h, WPARAM wParam)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// enumerate all child windows
///////////////////////////////////////////////////////////////////////////////
bool CALLBACK enumerateChildren(HWND handle, LPARAM lParam)
{
    if(lParam == WM_CLOSE)
    {
        ::SendMessage(handle, WM_CLOSE, 0, 0);  // close child windows
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// dialog procedure for About window
///////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_COMMAND:
        if(LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
            ::EndDialog(hwnd, 0);
        }
        break;
    }

    return false;
}

