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

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain() {};
        ~ControllerMain() {};

        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int close();
        int create();                               // for WM_CRERATE
        int size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

    private:
        //Win::Model model;
        //Win::View  view;

    };
}

#endif
