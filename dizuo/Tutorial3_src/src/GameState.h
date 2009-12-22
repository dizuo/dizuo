#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <Windows.h>

class CStateManager;

// Base class for the different states
// of the game.
class CGameState
{
public:
	// Constructor
	CGameState(CStateManager* pManager);
	// Destructor
	virtual ~CGameState();

	// The different 'events' functions. Child classes can 
	// implement the ones in which they are interested in.
	virtual void OnKeyDown(WPARAM ) { }
	virtual void OnKeyUp(WPARAM )   { }
	virtual void OnChar(WPARAM )   { }
	virtual void Update(DWORD )  { }
	virtual void Draw()  { }

	// Functions called when the state is entered or left
	// (transition from/to another state).
	virtual void EnterState()  { }
	virtual void LeaveState()   { }

protected:
	// Helper function to switch to a new active state.
	void ChangeState(CGameState* pNewState);

	// The state manager.
	CStateManager* m_pStateManager;
};

#endif  // _GAMESTATE_H_