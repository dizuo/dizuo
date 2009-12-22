#include "HighScoreState.h"
#include "MenuState.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;


CHighScoreState::CHighScoreState(CStateManager* pManager) 
  : CGameState(pManager), m_ulNewHighScore(0), m_bEnterName(false), 
    m_pFont(NULL), m_iNameIndex(0), m_vecHighScores()
{
	m_pFont = new CGameFont;
	m_pFont->CreateFont("Verdana", 20, FW_NORMAL);
	m_pCurrentName[0] = '\0';

	m_pBackgroundImg = CImage::CreateImage("MainBackground.png",TRectanglei(0,600,0,800));
	m_pTitleImg = CImage::CreateImage("ScoresTitle.png",TRectanglei(0,600,0,800));
	
	m_pEntriesBckgndImg = CImage::CreateImage("HighScoreItems.png",TRectanglei(70,99,0,600));
	m_pEnterNameImg = CImage::CreateImage("HighScoreItems.png",TRectanglei(0,39,0,150));
	m_pEnterNameBackImg = CImage::CreateImage("HighScoreItems.png",TRectanglei(40,69,0,350));
}

CHighScoreState::~CHighScoreState()
{
}

CHighScoreState* CHighScoreState::GetInstance(CStateManager* pManager)
{
	static CHighScoreState Instance(pManager);
	return &Instance;
}

void CHighScoreState::OnKeyDown(WPARAM wKey) 
{ 
	if (m_bEnterName)
	{
		switch(wKey)
		{
			// In case of a return, the new score should be added.
		case VK_RETURN:
			AddNewScore(m_pCurrentName, m_ulNewHighScore);
			m_ulNewHighScore = 0;
			m_bEnterName = false;
			m_iNameIndex = 0;
			m_pCurrentName[0] = '\0';
			break;
		case VK_BACK:
			// Remove one character
			if (m_iNameIndex>0)
			{
				m_iNameIndex--;
				m_pCurrentName[m_iNameIndex] = '\0';
			}
			break;
		}
	}
	else
	{
		switch(wKey)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			ChangeState(CMenuState::GetInstance(m_pStateManager));
			break;
		}
	}
}

void CHighScoreState::OnChar(WPARAM wChar) 
{ 
	if (m_bEnterName && (m_iNameIndex<25))
	{
		// Filter the characters for only alphabetical
		// characters.
		if ( (wChar>=64 && wChar<=91) ||
			 (wChar>=97 && wChar<=122))
		{
			m_pCurrentName[m_iNameIndex] = wChar;
			m_iNameIndex++;
			m_pCurrentName[m_iNameIndex] = '\0';
		}
	}
}

void CHighScoreState::Draw()  
{
	m_pBackgroundImg->BlitImage();
	m_pTitleImg->BlitImage();

	int yPos = 135;
	THighScoreTable::iterator iter = m_vecHighScores.begin();
	for (iter; iter!=m_vecHighScores.end(); iter++)
	{
		m_pEntriesBckgndImg->BlitImage(100,yPos);
		CTextControl txtEntry(m_pFont,TRectanglei(yPos,yPos+30,110,690));
		txtEntry.SetAlignement(CTextControl::TALeft);
		txtEntry.SetText(iter->strPlayer);
		txtEntry.Draw();
		stringstream ssScore;
		ssScore << iter->ulScore;
		txtEntry.SetAlignement(CTextControl::TARight);
		txtEntry.SetText(ssScore.str());
		txtEntry.Draw();
		yPos += 35;
	}

	// If the player should enter its name, we draw something
	// additional.
	if (m_bEnterName)
	{
	//	m_pFont->DrawText("Enter name:",50,550,1.0,1.0,1.0);
	//	m_pEntriesBckgndImg->BlitImagePart(230,530,TRectanglei(0,30,0,350));
		m_pEnterNameImg->BlitImage(50,525);
		m_pEnterNameBackImg->BlitImage(230,530);
		m_pFont->DrawText(m_pCurrentName,250,550,1.0,1.0,1.0);
	}
}

void CHighScoreState::EnterState()
{
	// Clear the high-score table
	m_vecHighScores.clear();
	ifstream inputFile("HighScores.txt");
	if (inputFile.fail())
	{
		if (m_ulNewHighScore)
			m_bEnterName = true;
		return;
	}

	// Read all entries from the file
	while (!inputFile.eof())
	{
		HighScoreData newScore;
		inputFile >> newScore.strPlayer >> newScore.ulScore;
		m_vecHighScores.push_back(newScore);
	}
	// Sort the table
	sort(m_vecHighScores.begin(), m_vecHighScores.end());

	// Check if we have a new high-score that should be
	// added in the table. If yes, m_bEnterName is set
	// to true.
	ULONG lastScore = 0;
	if (m_vecHighScores.size())
		lastScore = m_vecHighScores[m_vecHighScores.size()-1].ulScore;
	if (m_ulNewHighScore && m_ulNewHighScore>lastScore)
		m_bEnterName = true;
}

void CHighScoreState::SaveScores()
{
	// Create the file
	ofstream outputFile("HighScores.txt");
	if (outputFile.fail())
		return;

	// Write all the entries in the file.
	THighScoreTable::iterator iter = m_vecHighScores.begin();
	for (iter; iter != m_vecHighScores.end(); iter++)
	{
		outputFile << iter->strPlayer << " " << iter->ulScore;
	}
}

void CHighScoreState::AddNewScore(const std::string& strName, ULONG ulScore)
{
	// Create a new high-score and push it into the table 
	HighScoreData newData;
	newData.strPlayer = strName;
	newData.ulScore = ulScore;
	m_vecHighScores.push_back(newData);
	
	// Sort the table
	sort(m_vecHighScores.begin(), m_vecHighScores.end());

	// If too much elements, remove the last one.
	while (m_vecHighScores.size() > 10)
		m_vecHighScores.pop_back();

	SaveScores();
}
