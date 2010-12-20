#include <stdlib.h>
#include <windows.h>
#include <gl/glut.h>
#include <iostream> 
#include <fstream>  
#include "glut.h"
#include "Camera2d.h"
#include "Camera3d.h"
using namespace std;

// Programa Principal 

int main(void)
{
     glut gl;
     gl.init();
	 gl.setCamera();
	 gl.setViewPort(0,0,800,600);
	 gl.registraDraw();    
	 gl.registraKeyboard();

     gl.loop();    
}
