#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameState.h"
#include "GameFont.h"
#include "PlayState.h"

// Specialization of the CGameState class for 
// the menu state. This displays a menu in which
// the player can start a new game, continue an 
// existing game, see the high-scores or exit the game.
class CMenuState : public CGameState
{
public:
	~CMenuState();

	void OnKeyDown(WPARAM wKey);
	void Draw();
	void EnterState();

	static CMenuState* GetInstance(CStateManager* pManager);

protected:
	CMenuState(CStateManager* pManager);

private:
	// The player went up or down in 
	// the menu
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	CGameFont* m_pFont;
	// Index of the current selected menu item
	int m_iCurrentSelection;

	// A pointer to the current active game (if any).
	CPlayState* m_pCurrentGame;

	// The background and title images
	TImagePtr m_pBackgroundImg;
	TImagePtr m_pTitleImg;

	// The images of the menu items (normal and
	// selected).
	TImagePtr m_pItemBckgndNormal;
	TImagePtr m_pItemBckgndSelected;

	// The text controls of the different entries.
	CTextControl* m_pNewGameText;
	CTextControl* m_pResumeGameText;
	CTextControl* m_pScoresText;
	CTextControl* m_pExitText;
};

#endif  // _MENUSTATE_H_