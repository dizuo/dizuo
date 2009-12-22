#ifndef _TEXTCONTROL_H_
#define _TEXTCONTROL_H_

#include <string>
#include "Rectangle.h"
#include "GameFont.h"

// Utility class that display text with an alignment
// in a specified region.
class CTextControl
{
public:
	CTextControl(CGameFont* pFont, const TRectanglei& rectPosition);
	~CTextControl();

	// The text alignment
	enum ETextAlignement
	{
		TALeft,
		TACenter,
		TARight
	};

	void SetText(const std::string& strText)  { m_strText = strText; }
	void SetAlignement(ETextAlignement Alignement) { m_Alignement = Alignement; }
	void Draw();
	void SetTextColor(GLfloat fRed, GLfloat fGreen, GLfloat fBlue);

private:
	// The font used for the text.
	CGameFont* m_pFont;
	// The rectangle in which the text is diplayed.
	TRectanglei m_rectPosition;
	// The text to display.
	std::string m_strText;
	// The alignment of the text.
	ETextAlignement m_Alignement;

	// The color of the text.
	GLfloat m_fRed;
	GLfloat m_fGreen;
	GLfloat m_fBlue;
};

#endif  _TEXTCONTROL_H_