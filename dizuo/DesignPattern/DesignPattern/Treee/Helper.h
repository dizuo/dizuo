#ifndef HELPER_H_
#define HELPER_H_

#include <gl/glut.h>
namespace Helper
{
	void renderBitmapString2D(float x, float y, void *font, char *string)
	{
		char *c;
		// set position to start drawing fonts
		glRasterPos2f(x, y);
		// loop all the characters in the string
		for (c=string; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}
	}

	void renderBitmapString3D(float x, float y, float z, void* font, std::string str)
	{
		// set position to start drawing fonts
		glRasterPos3f(x, y, z);

		for (size_t i(0); i<str.size(); i++)
		{
			glutBitmapCharacter(font, str[i]);
		}
	}
}

#endif