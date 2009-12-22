#include "Application.h" 
#include "Exception.h"

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT)
{
	hPrevInstance;
	try
	{
		// Create the application class, 
		// parse the command line and
		// start the app.
		CApplication theApp(Instance);
		theApp.ParseCmdLine(lpCmdLine);
		theApp.Run();
	}
	catch(CException& e)
	{
		MessageBox(NULL,e.what(),"Error",MB_OK|MB_ICONEXCLAMATION);
	}

	return 0;
}