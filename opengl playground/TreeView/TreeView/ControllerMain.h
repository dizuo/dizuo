///////////////////////////////////////////////////////////////////////////////
// ControllerMain.h
// ================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2006-07-09
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_MAIN_H
#define WIN_CONTROLLER_MAIN_H

#include "Controller.h"
#include "View.h"

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain() {};
        ~ControllerMain() {};

        int create();
        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int contextMenu(HWND handle, int x, int y);
        int close();
        int notify(int id, LPARAM lParam);
        int mouseMove(WPARAM state, int x, int y);
        int lButtonUp(WPARAM state, int x, int y);

    private:
        //Win::Model model;
        Win::View  view;

    };
}

#endif
