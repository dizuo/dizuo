#include "draw.h"

draw::draw(void)
{
  
}

draw::~draw(void)
{
}

void draw::desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha o teapot com a cor corrente (wire-frame)
	glutWireTeapot(30.0f);

	// Executa os comandos OpenGL
	glutSwapBuffers();


}
