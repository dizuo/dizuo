#ifndef _GAMEFONT_H_
#define _GAMEFONT_H_

#include "Rectangle.h"
#include <string>
#include <Windows.h>
#include <GL\gl.h>

// Utility class used to draw text on the screen using a 
// specific font.
class CGameFont
{
public:
	// Default constructor
	CGameFont();
	// Default destructor
	~CGameFont();

	// Create the font with a specific height and weight.
	void CreateFont(const std::string& strTypeface, 
					int iFontHeight, 
					int iFontWeight);
	// Draw text on the screen at the specified location with
	// the specified colour.
	void DrawText(const std::string& strText, int XPos, 
				  int YPos, GLfloat iRed=1.0, 
				  GLfloat iGreen=1.0, GLfloat iBlue=1.0);

	// Returns the size of the text. The top and right fields
	// of the returned rectangle are set to 0.
	TRectanglei GetTextSize(const std::string& strText);

	static void SetDeviceContext(HDC hDevContext)  
	{ m_hDeviceContext = hDevContext; }

private:
	// The device context used to create the font.
	static HDC m_hDeviceContext;
	// The index of the base of the list.
	GLuint m_uiListBase;
	// The win32 font
	HFONT m_hFont;
};

#endif  // _GAMEFONT_H_