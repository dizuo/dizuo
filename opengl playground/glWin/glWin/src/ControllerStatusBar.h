///////////////////////////////////////////////////////////////////////////////
// ControllerStatusBar.h
// =====================
// Derived Controller class for Status bar window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2007-02-18
// UPDATED: 2007-02-18
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_STATUS_BAR_H
#define WIN_CONTROLLER_STATUS_BAR_H

#include "Controller.h"


namespace Win
{
    class ControllerStatusBar : public Controller
    {
    public:
        ControllerStatusBar();
        ~ControllerStatusBar() {};

        int size(int w, int h, WPARAM wParam);          // for WM_SIZE


    private:
    };
}

#endif
