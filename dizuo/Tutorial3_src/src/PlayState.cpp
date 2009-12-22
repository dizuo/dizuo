#include "PlayState.h"
#include "StateManager.h"
#include "MenuState.h"
#include "HighScoreState.h"
#include <sstream>

using namespace std;

CPlayState::CPlayState(CStateManager* pManager)
 : CGameState(pManager), m_pMatrix(NULL), m_pFont(NULL), 
   m_pComboControl(NULL), m_pScoreControl(NULL), 
   m_pLevelControl(NULL), m_pLinesControl(NULL),
   m_iTotalLines(0), m_iCurrentLevel(0), m_ulCurrentScore(0), 
   m_bGameOver(false)
{
	AddFontResource("01 Digitall.ttf");
	m_pMatrix = new CBlocksMatrix(this,280,34);
	m_pFont = new CGameFont;
	m_pFont->CreateFont("01 Digitall", 20, FW_NORMAL);

	m_pComboControl = new CComboControl(TRectanglei(330,450,50,235),m_pFont);
	m_pScoreControl = new CTextControl(m_pFont,TRectanglei(145,210,620,730));
	m_pScoreControl->SetAlignement(CTextControl::TACenter);
	m_pScoreControl->SetTextColor(1.0f,0.588f,0.039f);
	m_pLinesControl = new CTextControl(m_pFont,TRectanglei(320,385,620,730));
	m_pLinesControl->SetAlignement(CTextControl::TACenter);
	m_pLinesControl->SetTextColor(1.0f,0.588f,0.039f);
	m_pLevelControl = new CTextControl(m_pFont,TRectanglei(500,565,620,730));
	m_pLevelControl->SetAlignement(CTextControl::TACenter);
	m_pLevelControl->SetTextColor(1.0f,0.588f,0.039f);

	m_pBackgroundImg = CImage::CreateImage("PlayBckgnd.png",TRectanglei(0,600,0,800));
}

CPlayState::~CPlayState()
{
	if (m_pMatrix)
	{
		delete m_pMatrix;
		m_pMatrix = NULL;
	}

	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	RemoveFontResource("01 Digitall.ttf");
}

CPlayState* CPlayState::GetInstance(CStateManager* pManager)
{
	static CPlayState Instance(pManager);
	return &Instance;
}

void CPlayState::Reset()
{
	m_iTotalLines = 0;
	m_iCurrentLevel = 0; 
	m_ulCurrentScore = 0;
	m_bGameOver = false;
	m_pMatrix->Reset();
	m_pComboControl->Reset();
}

void CPlayState::OnKeyDown(WPARAM wKey)
{
	switch (wKey)
	{
	case VK_UP:
		if (!m_bGameOver)
			m_pMatrix->ShapeRotate();
		break;
	case VK_DOWN:
		if (!m_bGameOver)	
			m_pMatrix->ShapeDown();
		break;
	case VK_LEFT:
		if (!m_bGameOver)
			m_pMatrix->ShapeLeft();
		break;
	case VK_RIGHT:
		if (!m_bGameOver)
			m_pMatrix->ShapeRight();
		break;
	case VK_ESCAPE:
		ChangeState(CMenuState::GetInstance(m_pStateManager));
		break;
	case VK_RETURN:
		if (m_bGameOver)
		{
			CHighScoreState* pHighScores = 
				CHighScoreState::GetInstance(m_pStateManager);
			pHighScores->SetNewHighScore(m_ulCurrentScore);
			ChangeState(pHighScores);
		}
	}
}

void CPlayState::Update(DWORD dwCurrentTime)
{
	if (!m_bGameOver)
	{
		m_pMatrix->Update(dwCurrentTime);
		m_pComboControl->Update(dwCurrentTime);
	}
}

void CPlayState::Draw()  
{ 
	m_pBackgroundImg->BlitImage(0,0);

	m_pMatrix->Draw();

	stringstream ssScore;
	ssScore << m_ulCurrentScore;
	m_pScoreControl->SetText(ssScore.str());
	m_pScoreControl->Draw();

	stringstream ssLines;
	ssLines << m_iTotalLines;
	m_pLinesControl->SetText(ssLines.str());
	m_pLinesControl->Draw();

	stringstream ssLevel;
	ssLevel << m_iCurrentLevel;
	m_pLevelControl->SetText(ssLevel.str());
	m_pLevelControl->Draw();

	if (m_pMatrix->GetNextShape())
		m_pMatrix->GetNextShape()->DrawOnScreen(TRectanglei(165,220,80,225));

	m_pComboControl->Draw();
	if (m_bGameOver)
	{
		// In game over, we draw a semi-transparent black screen on top
		// of the background. This is possible because blending has 
		// been enabled.
		glColor4f(0.0,0.0,0.0,0.5);
		// Disable 2D texturing because we want to draw a non
		// textured rectangle over the screen.
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glVertex3i(0,0,0);
		glVertex3i(0,600,0);
		glVertex3i(800,600,0);
		glVertex3i(800,0,0);
		glEnd();
		glEnable(GL_TEXTURE_2D);

		m_pFont->DrawText("GAME OVER",340,200);
		m_pFont->DrawText("Press Enter to continue",285,300);
	}

}

void CPlayState::OnStartRemoveLines()
{
	m_pComboControl->Pause();
}

void CPlayState::OnLinesRemoved(int iLinesCount)
{
	m_iTotalLines += iLinesCount;
	int comboMultiplier = m_pComboControl->GetMultiplier();
	switch (iLinesCount)
	{
	case 1:
		m_ulCurrentScore += (m_iCurrentLevel+1) * 40 * comboMultiplier;
		break;
	case 2:
		m_ulCurrentScore += (m_iCurrentLevel+1) * 100 * comboMultiplier;
		break;
	case 3:
		m_ulCurrentScore += (m_iCurrentLevel+1) * 300 * comboMultiplier;
		break;
	case 4:
		m_ulCurrentScore += (m_iCurrentLevel+1) * 1200 * comboMultiplier;
		break;
	}

	if (m_iTotalLines/10 > m_iCurrentLevel)
	{
		m_iCurrentLevel++;
		int iNewUpdateRate = (int)(m_pMatrix->GetTetradUpdate() * 0.8);
		m_pMatrix->SetTetradUpdate(iNewUpdateRate);
	}
	m_pComboControl->IncreaseMultiplier();
	m_pComboControl->Unpause();
}

void CPlayState::OnMatrixFull()
{
	m_bGameOver = true;
	m_pComboControl->Pause();
}
