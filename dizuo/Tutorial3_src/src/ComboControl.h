#ifndef _COMBOCONTROL_H_
#define _COMBOCONTROL_H_

#include "GameFont.h"
#include "Image.h"

// Class which handles the combo control: this
// control shows a decreasing time bar when a line
// has been completed. If a new line is completed 
// before time is over, a multiplier is applied to the 
// points gained for the line(s) completed. The higher
// the multiplier, the fastest time will decrease.
class CComboControl
{
public:
	CComboControl(const TRectanglei& rectControl, CGameFont* pFont);
	~CComboControl();

	// Updates and draw the control
	void Update(unsigned long dwCurrentTime);
	void Draw();

	// Increases the current multiplier and starts
	// a new timer which depends of the multiplier
	void IncreaseMultiplier();
	int GetMultiplier() const  { return m_iMultiplier; } 

	void Reset();
	void Pause();
	void Unpause();

private:
	// Returns the total time available for 
	// the current multiplier.
	int GetMultiplierTime() const;

	// The current combo multiplier
	int m_iMultiplier;
	// Time left for the current combo
	int m_iTimeLeft;
	// Total time for the current combo
	int m_iTotalTime;

	// The last time at which the control was updated
	unsigned long m_iLastUpdate;
	// true if the control is paused (time doesn't
	// decrease anymore)
	bool m_bPaused;

	// The font used by the control
	CGameFont* m_pFont;
	// The rectangle of the control
	TRectanglei m_rectControl;

	TImagePtr m_pProgressBorder;
	TImagePtr m_pProgressFill;
};

#endif  // _COMBOCONTROL_H_