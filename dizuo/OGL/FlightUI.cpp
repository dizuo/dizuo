#include	"FlightUI.h"
#include	<Windows.h>

FlightUI::FlightUI()
: m_fRotX(0.01)
, m_fRotY(0.01)
, m_fRotZ(0.01)
{
	m_fSpeed = 0.25;
	this->PositionCamera(0.0, 50.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0 );
}

FlightUI::~FlightUI()
{}

void FlightUI::StrafeCamera(float speed)
{	
	// Add the strafe vector to our position
	m_vPosition.x() += m_vStrafe.x() * speed;
	m_vPosition.y() += m_vStrafe.y() * speed;
	m_vPosition.z() += m_vStrafe.z() * speed;
	
	// Add the strafe vector to our view
	m_vView.x() += m_vStrafe.x() * speed;
	m_vView.y() += m_vStrafe.y() * speed;
	m_vView.z() += m_vStrafe.z() * speed;
}

void FlightUI::MoveCamera(float speed)
{
	gtl::Vec3f vVector = m_vView - m_vPosition;

	vVector.normalize();

	m_vPosition.x() += vVector.x() * speed;		
	m_vPosition.y() += vVector.y() * speed;
	m_vPosition.z() += vVector.z() * speed;		

	m_vView.x() += vVector.x() * speed;
	m_vView.y() += vVector.y() * speed;
	m_vView.z() += vVector.z() * speed;
}

void FlightUI::CheckForMovement()
{
	if(GetKeyState('W') & 0x80)			
		this->RotateView(m_fRotX, 1.0, 0.0, 0.0);

	if(GetKeyState('S') & 0x80)
		this->RotateView(-m_fRotX, 1.0, 0.0, 0.0);

	if(GetKeyState('A') & 0x80) {
		this->RotateView(m_fRotY, 0.0, 1.0, 0.0);
	}

	if(GetKeyState('D') & 0x80) {
		this->RotateView(-m_fRotY, 0.0, 1.0, 0.0);
	}

	if(GetKeyState('Q') & 0x80)
		this->RotateView(m_fRotZ, 0.0, 0.0, 1.0);
	
	if(GetKeyState('E') & 0x80)
		this->RotateView(-m_fRotZ, 0.0, 0.0, 1.0);


	if(GetKeyState(VK_SPACE) & 0x80)
		this->PositionCamera(0.0, 100.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0 );
	
	if(GetKeyState(VK_UP) & 0x80)
		this->MoveCamera(m_fSpeed);				

	if(GetKeyState(VK_DOWN) & 0x80)
		this->MoveCamera(-m_fSpeed);				

	if(GetKeyState(VK_LEFT) & 0x80)
		this->StrafeCamera(-m_fSpeed);

	if(GetKeyState(VK_RIGHT) & 0x80)
		this->StrafeCamera(m_fSpeed);
}

void FlightUI::Apply()
{
	this->Look();
}

void FlightUI::Update()
{
	this->_Update();
	this->CheckForMovement();
}