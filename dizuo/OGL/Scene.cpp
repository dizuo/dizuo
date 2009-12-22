//----------------------------------------------------------------------------
//[3/2/2009 RenYaFei]
//----------------------------------------------------------------------------
/*************************************************************/
/**					Filename: SCENE.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  05/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**   3/2/2009 RenYaFei									
					-	1.将字体利用正交变换显示，			
						m_Font->AddFontElem();			
						前面两个参数直接就是字体在窗口中的位置，
						因为正交投影坐标原点位于左下角
					-	2.在Scene类中增加了m_iWindowWid, 
						m_iWindowHigh,保存窗口大小			**/
/**					All Rights Reserved.					**/
/*************************************************************/
#include	"Scene.h"
#include	"Texture.h"
#include	"Timer.h"

#include	"Logger.h"

#include	"CameraMgr.h"
#include	"WalkUI.h"
#include	"DriveUI.h"
#include	"FlightUI.h"

#include	<iostream>
#include	<iomanip>
#include	<string>
#include	<Windows.h>

using std::string;

char* Scene::m_Control[FONT_NUM] = {
	#include "controls.h"
};

Scene::Scene()
	:m_ApplyMode(SCENE)
	, m_Track(NULL)
	, m_iWindowWid(1024)
	, m_iWindowHigh(768)
{
	m_Track = new TrackUI;

	m_Sound = new CSound;
	
	m_Sky	= new CSkyBox;
	m_Wall  = new Wall;
	m_Ground= new Ground;
	
	m_House	 = new Obj_3DS;
	m_Weapon = new Obj_3DS;

	m_Role  = new Role;
	m_Font	= new Font;

	m_vCheckOffset.setValue(6.0, 6.0, 6.0);
}

Scene::~Scene()
{
	SAFE_DELETE(m_Track);

	SAFE_DELETE(m_Sound);

	SAFE_DELETE(m_Sky);
	SAFE_DELETE(m_Wall);
	SAFE_DELETE(m_Ground);
	SAFE_DELETE(m_House);
	SAFE_DELETE(m_Weapon);

	SAFE_DELETE(m_Role);
	SAFE_DELETE(m_Font);

	CTextureManager::FreeInstance();
	CTimer::FreeInstance();
}

bool Scene::SceneInit()
{
	CameraMgr::GetInstance()->add<WalkUI>( string("walk"), true);
	CameraMgr::GetInstance()->add<DriveUI>( string("drive"), false);
	CameraMgr::GetInstance()->add<FlightUI>( string("flight"), false);
	
	m_Sky->Init();

	m_Wall->Init();
	m_Wall->SetWall(600, 400, 600);
	
	m_Ground->Init();

	m_House->Load("Data/3DS/House/fw43_lowpoly_n1.3ds");
	m_House->Scalev() = gtl::Vec3f(50.0, 50.0, 50.0);
	m_House->SetPos(10.0, m_Ground->GetPos().y(), 10.0);

	m_Weapon->Load("Data/3DS/95/mp5.3DS");
	m_Weapon->Scalev() = gtl::Vec3f(0.05, 0.05, 0.05);
	m_Weapon->SetPos(2.0, -2.0, -6.0);

	// Get the singleton of CTexture and CTimer
	CTextureManager::GetInstance()->Initialize();
	CTimer::GetInstance()->Initialize();

	m_Font->DeActive();
	m_Font->SetFontType(std::string("Helvetica 18"));
	m_Font->SetPos(0.0, 0.0, -2.0);
	//正交映射坐标从窗口左下角开始，原点位于左下角
	for (size_t i(0); i<FONT_NUM; i++)
		m_Font->AddFontElem(0.0, m_iWindowHigh-70.0-i*25.0,  0.0, m_Control[i]);
	
	return m_Role->Init();
}

void Scene::SceneDisplay()
{
	if(m_Weapon->m_bActive)
		ShowCursor( false );
	else
		ShowCursor( true );

	CTimer::GetInstance()->Update();
	float timesec = CTimer::GetInstance()->GetTimeMSec() / 1000.0;
	std::cout << "\r   fps = " << std::setprecision(4) << CTimer::GetInstance()->GetFps();

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	{
		// Camera Transform...
		Apply();

		// Draw the Sky
		m_Sky->Draw();
		m_Wall->Draw();

		glPushMatrix();
		{
			glTranslatef(m_Ground->GetPos().x(), m_Ground->GetPos().y(), m_Ground->GetPos().z());
			m_Ground->Draw();
		}glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(m_House->GetPos().x(), m_House->GetPos().y(), m_House->GetPos().z());
			glScalef(m_House->Scalev().x(), m_House->Scalev().y(), m_House->Scalev().z());

			if(m_ApplyMode == HOUSE)
				m_Track->Apply();
			// Draw the house.
			m_House->Draw();
		}glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(m_Role->GetPos().x(), m_Role->GetPos().y(), m_Role->GetPos().z());
			glRotatef(-m_Role->GetDir(), 0.0, 1.0, 0.0);
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
			
			m_Role->Move();
			if (m_ApplyMode == ROLE){
				m_Track->Apply();
				m_Role->Stop();
			}

			// Draw models
			m_Role->SetTimSec(timesec);
			m_Role->Draw();
		}glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(5.0, 5.0, 3.0);
			if(m_ApplyMode == TEAPOT)
				m_Track->Apply();
			glScalef(2.0, 2.0, 2.0);
			glutSolidTeapot(1.0);
		}glPopMatrix();
	}glPopMatrix();


	//////////////////////////////////////////////////////////////////////////
	// Weapon : mp5
	glPushMatrix();
	{
		glTranslatef(m_Weapon->GetPos().x(), m_Weapon->GetPos().y(), m_Weapon->GetPos().z());
 		if(m_ApplyMode == GUN)
 			m_Track->Apply();
		
		glPushMatrix();
		{
			glRotatef(180., 0.0, 1.0, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			glScalef(0.1, 0.1, 0.1);
			if(m_Weapon->m_bActive)
				m_Weapon->DrawModel();
		}glPopMatrix();
	}glPopMatrix();


	//////////////////////////////////////////////////////////////////////////
	// FONT
	glMatrixMode(GL_PROJECTION);  // Change Matrix Mode to Projection
	glLoadIdentity();             // Reset View
	glOrtho(0, m_iWindowWid, 0, m_iWindowHigh, 0, 100);
	glMatrixMode(GL_MODELVIEW);   // Change Projection to Matrix Mode
	glLoadIdentity();
	glPushMatrix();
	{
		glTranslatef(m_Font->GetPos().x(), m_Font->GetPos().y(), m_Font->GetPos().z());
		if(m_Font->m_bActive)
			m_Font->Draw();
	}glPopMatrix();

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluPerspective(80.0,(GLdouble)1024/(GLdouble)768,0.001, 2000.0);
}
void Scene::KeyboardHandle()
{
	if(GetKeyState(VK_ESCAPE) & 0x80)
		exit(1);
	
	if(GetKeyState('p') & 0x80 || GetKeyState('P') & 0x80)
		m_Sound->Play(CSound::MOVE);
	
	if(GetKeyState('c') & 0x80 || GetKeyState('C') & 0x80)
		m_ApplyMode = (m_ApplyMode+1) % 5;

	if( GetKeyState('h') & 0x80 
		|| GetKeyState('H') & 0x80
		|| GetKeyState(VK_TAB) & 0x80 )
		m_Font->m_bActive = !m_Font->m_bActive;

	if( GetKeyState('1') & 0x80)
		CameraMgr::GetInstance()->activate( string("walk") );
	
	if( GetKeyState('2') & 0x80)
		CameraMgr::GetInstance()->activate( string("drive") );

	if( GetKeyState('3') & 0x80)
		CameraMgr::GetInstance()->activate( string("flight") );

	if( GetKeyState('4') & 0x80) {
		m_Weapon->m_bActive = !m_Weapon->m_bActive;
	}
}

void Scene::OnMouseHandle(int btn, int state, int x, int y)
{
	m_Track->OnMouseHandle(btn, state, x, y);
}
void Scene::OnMotionHandle(int x, int y)
{
	m_Track->OnMotionHandle(x, y);
}

void Scene::Apply()
{
	gtl::Vec3f vView, vPos;
	switch(m_ApplyMode)
	{
	case SCENE:
		CameraMgr::GetInstance()->getActive()->Update();
		break;
	case HOUSE:
		vView = m_House->GetPos();
		vPos = vView + m_vCheckOffset;
		CameraMgr::GetInstance()->getActive()->PositionCamera( vPos.x(), vPos.y(), vPos.z(),
																vView.x(), vView.y(), vView.z(),
																0.0, 1.0, 0.0 );
		break;
	case ROLE:
		vView = m_Role->GetPos();
		vPos = vView + m_vCheckOffset;
		CameraMgr::GetInstance()->getActive()->PositionCamera( vPos.x(), vPos.y(), vPos.z(),
																vView.x(), vView.y(), vView.z(),
																0.0, 1.0, 0.0 );
		break;
	default:
		CameraMgr::GetInstance()->getActive()->ComputeHome();
		break;
	}

	CameraMgr::GetInstance()->getActive()->Apply();
}

void Scene::ComputerHome()
{
	CameraMgr::GetInstance()->getActive()->ComputeHome();
}

void Scene::SetWindow(int x, int y)
{
	m_iWindowWid = x;
	m_iWindowHigh = y;
	CameraMgr::GetInstance()->getActive()->SetWnd(x, y);
	m_Track->SetWnd(x, y);
}