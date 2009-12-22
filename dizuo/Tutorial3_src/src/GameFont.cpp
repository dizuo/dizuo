#include "GameFont.h"
#include "Exception.h"

using namespace std;

HDC CGameFont::m_hDeviceContext = NULL;

CGameFont::CGameFont() : m_uiListBase(0)
{
}

CGameFont::~CGameFont()
{
	if (m_uiListBase)
		glDeleteLists(m_uiListBase,255);
	DeleteObject(m_hFont);
}

void CGameFont::CreateFont(const std::string& strTypeface, 
						   int iFontHeight,
						   int iFontWeight)
{
	if (!m_hDeviceContext)
	{
		string strError = "Impossible to create the font: ";
		strError += strTypeface;
		throw CException(strError);
		return;
	}

	// Ask openGL to generate a contiguous set of 255 display lists.
	m_uiListBase = glGenLists(255);
	if (m_uiListBase == 0)
	{
		string strError = "Impossible to create the font: ";
		strError += strTypeface;
		throw CException(strError);
		return;
	}

	// Create the Windows font
	m_hFont = ::CreateFont(-iFontHeight,
								  0,
								  0,
								  0,
								  iFontWeight,
								  FALSE,
								  FALSE,
								  FALSE,
								  ANSI_CHARSET,
								  OUT_TT_PRECIS,
								  CLIP_DEFAULT_PRECIS,
								  ANTIALIASED_QUALITY,
								  FF_DONTCARE|DEFAULT_PITCH,
								  strTypeface.c_str());
	if (m_hFont == NULL)
	{
		m_uiListBase = 0;
		string strError = "Impossible to create the font: ";
		strError += strTypeface;
		throw CException(strError);
		return;
	}

	// Select the newly create font into the device context (and save the previous
	// one).
	HFONT hOldFont = (HFONT)SelectObject(m_hDeviceContext, m_hFont);
	// Generate the font display list (for the 255 characters) starting
	// at display list m_uiListBase.
	wglUseFontBitmaps(m_hDeviceContext, 0, 255, m_uiListBase);	
	// Set the original font back in the device context
	SelectObject(m_hDeviceContext, hOldFont);	
}

TRectanglei CGameFont::GetTextSize(const std::string& strText)
{
	SIZE TextSize;
	HFONT hOldFont = (HFONT)SelectObject(m_hDeviceContext, m_hFont);
	// Retrieve the size of the text
	GetTextExtentPoint32(m_hDeviceContext,strText.c_str(),(int)strText.size(),&TextSize);
	SelectObject(m_hDeviceContext, hOldFont);	

	TRectanglei newTextSize(0,TextSize.cy,0,TextSize.cx);
	return newTextSize;
}

void CGameFont::DrawText(const std::string& strText, 
						 int XPos, int YPos,
						 GLfloat fRed, 
						 GLfloat fGreen, 
						 GLfloat fBlue)
{
	if (m_uiListBase == 0)
	{
		throw CException("Impossible to diplay the text.");
		return;
	}

	// Disable 2D texturing
	glDisable(GL_TEXTURE_2D);
	// Specify the current color
	glColor3f(fRed, fGreen, fBlue);
	// Specify the position of the text
	glRasterPos2i(XPos, YPos);

	// Push the list base value
	glPushAttrib (GL_LIST_BIT);
		// Set a new list base value. 
		glListBase(m_uiListBase);
		// Call the lists to draw the text.
		glCallLists((GLsizei)strText.size(), GL_UNSIGNED_BYTE, 
					(GLubyte *)strText.c_str());
    glPopAttrib ();

	// Switch back to white
	glColor3f(1.0, 1.0, 1.0);

	// Reenable 2D texturing
	glEnable(GL_TEXTURE_2D);
}
