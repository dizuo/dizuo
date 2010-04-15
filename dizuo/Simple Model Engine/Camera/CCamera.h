#ifndef CCAMERA_H_
#define CCAMERA_H_

#include "..//Define.h"

class CCamera
{
public:
	enum{ 
		BALL_RADIUS = 5,
		ZOOM_FACTIOR = 5
	};

public:
	CCamera();
	~CCamera();

	void Reset();
	void PositionCamera();
	void ResetView(int w, int h);
	
	void PanCamera(float deltax, float deltay);		//MoveCamera
	void ZoomCamera(float zoomRate);				//ZoomCamera
	void RotateCamera(float deltax, float deltay);	//RoteCamera

	const RVec2& GetCameraRotVec() const { return m_vRotVec; }

	void GetWorldCoord(int ix, int iy, double fz, RVec3& coord);

protected:
	int		m_iScreenWidth;
	int		m_iScreenHeight;
	int		m_bPerspective;

	int		m_iViewport[4];
	double	m_dProjectionMatrix[16];
	double  m_dModelViewMatrix[16];

	float	m_fAspect;
	float	m_fFovY;
	float	m_fNear;
	float   m_fFar;

	// The camera's position
	RVec3	m_vPosition;					
	// The camera's view
	RVec3	m_vView;						
	// The camera's up vector
	RVec3	m_vUpVector;

	//////////////////////////////////////////////////////////////////////////
	float	m_fZoomRate;		//Zoom
	RVec3	m_vPanVec;			//Pan vector
	RVec2	m_vRotVec;		//Rotate angle
	//////////////////////////////////////////////////////////////////////////
	void	_UpdateCameraPosition();

};

#endif