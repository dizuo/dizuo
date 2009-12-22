/*************************************************************/
/**					Filename: WALKUI.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  05/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef WALKGUI_H
#define WALKGUI_H

#include	"Camera.h"

class WalkUI : public CCamera
{
public:
	WalkUI();
	~WalkUI();
	
	void Update();
	void CheckForMovement();
	void Apply();

protected:
	void SetViewByMouse();

	float m_fHighOffset;
};
#endif