/*************************************************************/
/**					Filename: DRIVEUI.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  09/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef __DRIVEUI_H
#define __DRIVEUI_H

#include	"Camera.h"

class DriveUI : public CCamera
{
public:
	DriveUI();
	~DriveUI();

	void Update();
	void CheckForMovement();
	void Apply();
protected:
	float	m_fRotSpeed;
};

#endif