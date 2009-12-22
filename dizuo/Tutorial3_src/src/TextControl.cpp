#include "TextControl.h"

CTextControl::CTextControl(CGameFont* pFont, const TRectanglei& rectPosition)
  : m_pFont(pFont), m_rectPosition(rectPosition), m_strText(""), 
    m_fRed(1.0), m_fGreen(1.0), m_fBlue(1.0)
{

}

CTextControl::~CTextControl()
{
}

void CTextControl::Draw()
{
	TRectanglei textSize = m_pFont->GetTextSize(m_strText);
	int yOffset = m_rectPosition.GetHeight()/2+textSize.GetHeight()/2;
	int xOffset = 0;
	switch (m_Alignement)
	{
	case TALeft:
		break;
	case TACenter:
		xOffset = (m_rectPosition.GetWidth()-textSize.GetWidth())/2;
		break;
	case TARight:
		xOffset = m_rectPosition.GetWidth()-textSize.GetWidth();
		break;
	}

	// TODO: check why we need to correct the ypos ?
	int xPos = m_rectPosition.m_Left + xOffset;
	int yPos = m_rectPosition.m_Top + yOffset - 5;
	m_pFont->DrawText(m_strText,xPos,yPos,m_fRed,m_fGreen,m_fBlue);
}

void CTextControl::SetTextColor(GLfloat fRed, 
								GLfloat fGreen, 
								GLfloat fBlue)
{
	m_fRed = fRed;
	m_fGreen = fGreen;
	m_fBlue = fBlue;
}
