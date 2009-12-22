#ifndef _HIGHSCORESTATE_H_
#define _HIGHSCORESTATE_H_

#include "GameState.h"
#include "GameFont.h"
#include "Image.h"
#include <string>
#include <vector>

// Specialization of the CGameState class for 
// the high scores state. This displays the high
// scores (player name+score). When a new high 
// score is available after a game, it lets the 
// player enters his name.
class CHighScoreState : public CGameState
{
public:
	~CHighScoreState();

	// Sets a new score: if this score should be 
	// part of the high scores, the user will need 
	// to enter his name.
	void SetNewHighScore(ULONG ulNewHighScore)  
	{ m_ulNewHighScore = ulNewHighScore; }

	// Implementation of specific events
	void OnKeyDown(WPARAM wKey);
	void OnChar(WPARAM wChar);
	void Draw();
	void EnterState();

	static CHighScoreState* GetInstance(CStateManager* pManager);

protected:
	CHighScoreState(CStateManager* pManager);

private:
	// Saves the current high scores
	void SaveScores();

	// Adds a new score in the high-score table and
	// insert it at the correct location.
	void AddNewScore(const std::string& strName, ULONG ulScore);

	// High-score data: score and player name.
	struct HighScoreData
	{
		std::string strPlayer;
		ULONG ulScore;

		// We have to sort in decreasing order, so the <
		// operator returns the opposite.
		bool operator< (const HighScoreData& other)
		{
			if  (this->ulScore > other.ulScore) 
				return true;
			return false;
		}
	};

	// The new high-score, if any.
	ULONG m_ulNewHighScore;
	// Mode in which the user has to enter his name.
	bool m_bEnterName;
	// Char array containing the name currently being entered.
	char m_pCurrentName[26];
	// The index of the next char to be entered.
	int m_iNameIndex;

	CGameFont* m_pFont;

	typedef std::vector<HighScoreData> THighScoreTable;
	// The high-score table.
	THighScoreTable m_vecHighScores;

	// The background and title images.
	TImagePtr m_pBackgroundImg; 
	TImagePtr m_pTitleImg; 
	// The image of the entries background
	TImagePtr m_pEntriesBckgndImg;
	// The 'Enter name' image and the background.
	TImagePtr m_pEnterNameImg; 
	TImagePtr m_pEnterNameBackImg; 
};

#endif  // _HIGHSCORESTATE_H_