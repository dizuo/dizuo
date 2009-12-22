#include "Camera.h"

#include "gtl/quat.hpp"
#include <iostream>
#include <GL/glut.h>
#include <Windows.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600								
#define SCREEN_DEPTH 16									
#define MOUSESENSITIVITY 1000

CCamera::CCamera()
: m_MouseSensitity(1000)
, m_fSpeed(0.5)
{
	m_vPosition.setValue(0.0, 0.0, 0.0);
	m_vView.setValue(0.0, 1.0, 0.5);
	m_vUpVector.setValue(0.0, 0.0, 1.0);
}

void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	m_vPosition.setValue(positionX, positionY, positionZ);
	m_vView.setValue(viewX, viewY, viewZ);
	m_vUpVector.setValue(upVectorX, upVectorY, upVectorZ);
}

//	This rotates the view around the position using an axis-angle rotation
void CCamera::RotateView(float angle, float x, float y, float z)
{
 	gtl::Vec3f vNewView;
 
 	// Get the view vector (The direction we are facing)
 	gtl::Vec3f vView = m_vView - m_vPosition;		
	gtl::Matrix3f RotMatrix;

	RotMatrix.setRotate(gtl::Vec3f(x, y, z), angle);
	RotMatrix.multMatrixVec(vView, vNewView);

 	m_vView = m_vPosition + vNewView;
}

void CCamera::StrafeCamera(float speed)
{	
	// Add the strafe vector to our position
	m_vPosition.x() += m_vStrafe.x() * speed;
	m_vPosition.z() += m_vStrafe.z() * speed;

	// Add the strafe vector to our view
	m_vView.x() += m_vStrafe.x() * speed;
	m_vView.z() += m_vStrafe.z() * speed;
}

void CCamera::MoveCamera(float speed)
{
	gtl::Vec3f vVector = m_vView - m_vPosition;

	vVector.normalize();
	
	m_vPosition.x() += vVector.x() * speed;		
	m_vPosition.z() += vVector.z() * speed;		
	m_vView.x() += vVector.x() * speed;			
	m_vView.z() += vVector.z() * speed;			
}

//	This updates the camera's view and strafe vector
void CCamera::_Update() 
{
	gtl::Vec3f sub = m_vView - m_vPosition;
	gtl::Vec3f vCross = sub.cross(m_vUpVector);
	vCross.normalize();

	m_vStrafe = vCross;
}

void CCamera::Look()
{
	gluLookAt(m_vPosition.x(), m_vPosition.y(), m_vPosition.z(),	
			  m_vView.x(),	 m_vView.y(),     m_vView.z(),	
			  m_vUpVector.x(), m_vUpVector.y(), m_vUpVector.z() );
}

// reset the game state 
void CCamera::ComputeHome()
{
	m_vPosition.setValue(0.0, 0.0, 0.0);
	m_vView.setValue(0.0, 1.0, 0.0);
	m_vUpVector.setValue(0.0, 0.0, 1.0);

	// 以默认的方式
	this->PositionCamera(0.0, 2.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0 );
	this->MoveCamera( 1.0 );
}

//////////////////////////////////////////////////////////////////////////
// BUG：RotateView 单位是角度.