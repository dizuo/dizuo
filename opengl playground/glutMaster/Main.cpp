#include "glutMaster.h"
#include "demoWindow.h"
#include "glutWindow.h"

GlutMaster*		glutMaster = 0;
DemoWindow*		firstWindow = 0;
DemoWindow*		secondWindow = 0;

void main(void)
{

	glutMaster   = new GlutMaster();    

	firstWindow  = new DemoWindow(glutMaster,
		200, 200,    // height, width
		200, 100,    // initPosition (x,y)
		"First Window"); // title

	secondWindow = new DemoWindow(glutMaster,
		500, 500,    // height, width
		200, 400,    // initPosition (x,y)
		"Second Window"); // title

	secondWindow->StartSpinning(glutMaster);   // enable idle function

	glutMaster->CallGlutMainLoop();
}
