#include "ComboControl.h"
#include <sstream>

using namespace std;


CComboControl::CComboControl(const TRectanglei& rectControl, CGameFont* pFont) 
  : m_iMultiplier(1), m_iTimeLeft(0), m_iTotalTime(0), 
    m_iLastUpdate(0), m_bPaused(false), m_pFont(pFont),
	m_rectControl(rectControl)
{
	m_pProgressBorder = CImage::CreateImage("ProgressBar.png",TRectanglei(0,24,0,126));
	m_pProgressFill = CImage::CreateImage("ProgressBar.png",TRectanglei(27,51,0,126));
}

CComboControl::~CComboControl()
{
	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

void CComboControl::Pause()
{
	m_bPaused = true;
}

void CComboControl::Unpause()
{
	m_iLastUpdate = GetCurrentTime();
	m_bPaused = false;
}

void CComboControl::Update(unsigned long dwCurrentTime)
{
	if (m_iMultiplier == 1)
		return;
	if (m_bPaused)
		return;

	int dwTimeDiff = dwCurrentTime - m_iLastUpdate;
	if (dwTimeDiff >= m_iTimeLeft)
	{
		m_iMultiplier--;
		if (m_iMultiplier != 1)
			m_iTimeLeft = m_iTotalTime = GetMultiplierTime();
		else
			m_iTimeLeft = m_iTotalTime = 0;
	}
	else
		m_iTimeLeft -= dwTimeDiff;
	m_iLastUpdate = dwCurrentTime;
}

void CComboControl::Draw()
{
	stringstream ssText;
	ssText << "Multiplier: X" << m_iMultiplier;
	m_pFont->DrawText(ssText.str(),m_rectControl.m_Left+25,
					  m_rectControl.m_Top+60,1.0f,0.588f,0.039f);

	float barPercent = 	0;
	if (m_iTotalTime!=0)
		barPercent = (float)m_iTimeLeft/m_iTotalTime;
	else
		barPercent = 0;

	TRectanglei fill(0,24,0,(int)(126*barPercent));
	m_pProgressFill->BlitImagePart(85,405,fill);
	m_pProgressBorder->BlitImage(85,405);
}

void CComboControl::IncreaseMultiplier()
{
	if (m_iMultiplier<10)
		m_iMultiplier++;
	m_iTotalTime = m_iTimeLeft = GetMultiplierTime();
	m_iLastUpdate = GetCurrentTime();
}

int CComboControl::GetMultiplierTime() const
{
	return 20000 - (m_iMultiplier-2)*2000;
}

void CComboControl::Reset()
{
	m_iMultiplier = 1;
	m_iTotalTime = m_iTimeLeft = 0;
	m_iLastUpdate = 0;
}
