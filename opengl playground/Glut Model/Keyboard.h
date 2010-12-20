#pragma once

class Keyboard
{
public:
	Keyboard(void);
	~Keyboard(void);

	static void Teclado(unsigned char key, int x, int y);
    void Verifica(int tecla);


private:
	 unsigned char KEY[250];
};
