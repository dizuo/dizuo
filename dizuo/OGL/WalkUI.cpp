#include	"gtl/vec2.hpp"
#include	"gtl/vec3.hpp"
#include	"WalkUI.h"
#include	<Windows.h>


WalkUI::WalkUI()
{
	m_fSpeed = 0.2;
	ComputeHome();
}

WalkUI::~WalkUI()
{}

void WalkUI::Update()
{
	this->_Update();
	this->CheckForMovement();
	this->SetViewByMouse();
}

void WalkUI::SetViewByMouse()
{
	int middleX = m_iWndWid >> 1;				
	int middleY = m_iWndHet >> 1;				
	float angleY = 0.0f;							
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;

	POINT mousePos;
	GetCursorPos(&mousePos);

	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) ;

	//glutSetCursor( GLUT_CURSOR_FULL_CROSSHAIR );
	SetCursorPos(middleX, middleY);
	//ShowCursor(FALSE);

	angleY = (float)( (middleX - mousePos.x) ) / m_MouseSensitity;		
	angleZ = (float)( (middleY - mousePos.y) ) / m_MouseSensitity;		

	currentRotX -= angleZ;  

	gtl::Vec3f sub = m_vView - m_vPosition;
	gtl::Vec3f vAxis = sub.cross(m_vUpVector);
	vAxis.normalize();

	RotateView(angleZ, vAxis.x(), vAxis.y(), vAxis.z());
	RotateView(angleY, 0, 1, 0);
}

void WalkUI::CheckForMovement()
{
	if(GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)			
		this->MoveCamera(m_fSpeed);				

	if(GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
		this->MoveCamera(-m_fSpeed);				
	
	if(GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)			
		this->StrafeCamera(-m_fSpeed);

	if(GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)
		this->StrafeCamera(m_fSpeed);

	if(GetKeyState(VK_SPACE) & 0x80 || GetKeyState(' ') & 0x80)
		this->ComputeHome();
}

void WalkUI::Apply()
{
	this->Look();
}

