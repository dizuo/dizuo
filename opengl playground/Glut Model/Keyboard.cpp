
#include <iostream> 
#include "Keyboard.h"
#include <gl/glut.h>
using namespace std;


Keyboard::Keyboard(void)
{
}

Keyboard::~Keyboard(void)
{
}

void Keyboard::Teclado(unsigned char key, int x, int y)
{
	
	
    switch (key) {
            case 'R': 
            case 'r':// muda a cor corrente para vermelho
                     glColor3f(1.0f, 0.0f, 0.0f);
					 break;
            case 'G':
            case 'g':// muda a cor corrente para verde
                     glColor3f(0.0f, 1.0f, 0.0f);
					 break;
	}

	glutPostRedisplay();
           
    
  
}

void Keyboard::Verifica(int tecla)
{
}
