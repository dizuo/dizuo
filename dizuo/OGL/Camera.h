/*************************************************************/
/**					Filename: CAMERA.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  15/05/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef _CAMERA_H
#define _CAMERA_H

// This is our camera class
#include "gtl/vec2.hpp"
#include "gtl/vec3.hpp"
#include "gtl/matrix3.hpp"

//#define kSpeed	5.5f
#define kSpeed	0.5f

class CCamera 
{
public:
	CCamera();	
	virtual ~CCamera(){};

	virtual void Update() = 0;
	virtual void CheckForMovement() = 0;
	virtual void Apply() = 0;

	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	void RotateView(float angle, float X, float Y, float Z);
	
	// This strafes the camera left or right depending on the speed (+/-) 
	virtual void StrafeCamera(float speed);
	// This will move the camera forward or backward depending on the speed
	virtual void MoveCamera(float speed);

	void _Update();
	// This uses gluLookAt() to tell OpenGL where to look
	void Look();
	
	// Reset the state
	void ComputeHome();

	// These are are data access functions for our camera's private data
	const gtl::Vec3f GetPos()	const	{	return m_vPosition;		};
	const gtl::Vec3f GetView() const	{	return m_vView;			};
	gtl::Vec3f Position()	{	return m_vPosition;		};
	gtl::Vec3f View()		{	return m_vView;			};
	gtl::Vec3f UpVector()	{	return m_vUpVector;		};
	gtl::Vec3f Strafe()		{	return m_vStrafe;		};

	void SetSpeed(float speed){	m_fSpeed = speed; }
	void SetWnd(int x, int y){m_iWndHet = y; m_iWndWid = x;}
	void SetMouseSensitity(float i) {m_MouseSensitity = i;} 

	float GetSpeed() const {return m_fSpeed;}
	float GetMouseSensitity() const {return m_MouseSensitity;}
protected:
	// The camera's position
	gtl::Vec3f m_vPosition;					
	// The camera's view
	gtl::Vec3f m_vView;						
	// The camera's up vector
	gtl::Vec3f m_vUpVector;		
	// The camera's strafe vector
	gtl::Vec3f m_vStrafe;

	int		m_iWndWid;
	int		m_iWndHet;
	float	m_MouseSensitity;
	float	m_fSpeed;
};
#endif

//
// 细分：
//	Walk:
//		两个自由度.
//	Drive: 
//		不用鼠标：方向键，left,right控制旋转. up,down控制进退,
//		一个自由度.
//	Flight: 
//		三个自由度控制.
