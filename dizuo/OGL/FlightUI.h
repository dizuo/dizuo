/*************************************************************/
/**					Filename: FLIGHTUI.H		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  09/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef __FLIGHTUI_H
#define __FLIGHTUI_H

#include	"Camera.h"

class FlightUI : public CCamera
{
public:
	FlightUI();
	~FlightUI();

	virtual void StrafeCamera(float);
	virtual void MoveCamera(float speed);
	void Update();
	void CheckForMovement();
	void Apply();
protected:
	float	m_fRotX;
	float	m_fRotY;
	float	m_fRotZ;
};

#endif