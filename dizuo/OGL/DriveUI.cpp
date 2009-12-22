#include	"DriveUI.h"
#include	<Windows.h>

DriveUI::DriveUI()
: m_fRotSpeed(0.01)
{
	m_fSpeed = 0.35;
	ComputeHome();
}

DriveUI::~DriveUI()
{}

void DriveUI::CheckForMovement()
{
	if(GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)			
		this->MoveCamera(m_fSpeed);				

	if(GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
		this->MoveCamera(-m_fSpeed);

	if(GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		this->RotateView(m_fRotSpeed, 0.0, 1.0, 0.0);
	}

	if(GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		this->RotateView(-m_fRotSpeed, 0.0, 1.0, 0.0);
	}

	if(GetKeyState(VK_SPACE) & 0x80 || GetKeyState(' ') & 0x80)
		this->ComputeHome();
}

void DriveUI::Update()
{
	this->_Update();
	this->CheckForMovement();
}

void DriveUI::Apply()
{
	this->Look();
}