#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "GameState.h"

// Manages the different states of the game.
class CStateManager
{
public:
	// Default constructor
	CStateManager();
	// Default destructor
	~CStateManager();

	// Switches to another active state.
	void ChangeState(CGameState* pNewState)
	{
		if (m_pActiveState)
			m_pActiveState->LeaveState();
		m_pActiveState = pNewState;
		m_pActiveState->EnterState();
	}

	// Returns the current active state.
	CGameState* GetActiveState()  { return m_pActiveState; }

	// 'Events' function, they are simply redirected to
	// the active state.
	void OnKeyDown(WPARAM wKey);
	void OnKeyUp(WPARAM wKey);
	void OnChar(WPARAM wChar);
	void Update(DWORD dwCurrentTime);
	void Draw();

private:
	// Active State of the game (intro, play, ...)
	CGameState* m_pActiveState;		
};

#endif  // _STATEMANAGER_H_