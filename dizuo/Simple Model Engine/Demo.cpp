// Demo.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Define.h"
#include "Demo.h"

#include "Model/Model.h"

#include "Model/CCylinder.h"
#include "Model/CSphere.h"
#include "Model/CTorus.h"
#include "Model/CCone.h"

#include "Model/TwistApplyer.h"
#include "Model/WaveApplyer.h"

#include "Model/CExtrude.h"
#include "Model/CLathe.h"
#include "Model/CLoft.h"

#include "Curve/CCircle.h"
#include "Curve/CBspline.h"

#include <stdio.h>
#include <GL/glaux.h>

/////////////////////////////////////////////////////////////////////////////
// Global CDemoApp object

CDemoApp gApp;

/////////////////////////////////////////////////////////////////////////////
// CDemoApp construction/destruction

CDemoApp::CDemoApp()
	: m_iTexId(-1)
	, m_pRenderModel(0)
	, m_pRenderSpline(0)
	, m_bIsShowSpline(0)
{
	m_pRenderModel = new CCylinder(1.0, 4.0f, 4, 5);
	m_pRenderModel->Setup();
	
	CreateFlowerSpline();
}

CDemoApp::~CDemoApp()
{
}

//
// PreCreateWindow()
// Called before the main window is created. Override this function to change window creation params.
//
void CDemoApp::PreCreateWindow(WNDCLASSEX &wcex, DWORD &dwStyle, DWORD &dwExStyle, int &x, int &y, int &cx, int &cy)
{
	CApp::PreCreateWindow(wcex, dwStyle, dwExStyle, x, y, cx, cy);
	// TODO : Modify parameters to change default window style and/or size
}


//
// OnCommand()
// Override this function to handle WM_COMMAND messages sent to the main window.
//
BOOL CDemoApp::OnCommand(int nCmdID, int nEvent)
{
	switch (nCmdID)
	{
	case IDM_EXIT:
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;

	case IDM_ABOUT:
		ShowAboutDialog();
		break;
	case ID_VIEW_MOVE:
		m_hand_action = HA_MOVE;
		break;
	case ID_VIEW_ROTATE:
		m_hand_action = HA_ROTATE;
		break;
	case ID_VIEW_ZOOM:
		m_hand_action = HA_ZOOM;
		break;
	case ID_VIEW_RESET:
		//compute home
		m_Camera.Reset();
		SendMessage(m_hWnd, WM_PAINT,0,0);
		break;

	case ID_PRIMITIVES_CYLINDER:
		CreateCylinder();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case ID_PRIMITIVES_SPHERE:
		CreateSphere();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case ID_PRIMITIVES_CONE:
		CreateCone();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case ID_PRIMITIVES_TORUS:
		CreateTorus();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case ID_SPLINE_FLOWER:
		m_bIsShowSpline = !m_bIsShowSpline;

		CreateFlowerSpline();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case ID_SPLINE_DEWAVE:
		m_bIsShowSpline = !m_bIsShowSpline;

		CreateDeWaveSpline();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	case ID_SPLINE_SEASTAR:
		m_bIsShowSpline = !m_bIsShowSpline;

		CreateSeaStarSpline();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	//set the timer
	case ID_SPECIALEFFECT_TWIST:
		CreateTwistEffect();
		break;

	case ID_SPECIALEFFECT_WAVE:
		CreateWaveEffect();
		break;

	case ID_KINEMATIC_EXTRUDE:
		//display the extruding object...
		CreateExtrudeModel();
		break;

	case ID_KINEMATIC_LOFT:
		KillTimer(m_hWnd, TIMER1);
		CreateLoftModel();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	//坐标系
	case ID_COMPOUNDSCENE_AXIS:
		CreateCompoundAxis();
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		break;

	default:
		return CApp::OnCommand(nCmdID, nEvent); // Call default handler
	}
	return TRUE;
}

//
// WindowProc()
// Override this function to handle messages sent to the main window.
//
LRESULT CDemoApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CApp::WindowProc(hWnd, message, wParam, lParam); // Call default WindowProc
}

//
// InitScene()
// Called when the OpenGL RC has been created. Sets initial state for the OpenGL scene.
//
void CDemoApp::InitScene()
{
	// Lights properties
	//float ambientProperties[]  = {0.1f, 0.1f, 0.1f, 1.0f};
	float diffuseProperties[]  = {1.0f, 1.0f, 1.0f, 1.0f};
	float specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};

	//glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv( GL_LIGHT0, GL_DIFFUSE,diffuseProperties);
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glClearColor(0.9,0.9,0.9,1.0f);

	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);

	// Texture
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Default : lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Default : blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	//  glEnable(GL_CULL_FACE);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f );

	// Setup the textures...
	SetupTexture();

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
}

//
// KillScene()
// Called when the OpenGL RC is about to be destroyed.
//
void CDemoApp::KillScene()
{
	// TODO: Use KillScene to free resources.
}

//
// DrawScene()
// Called each time the OpenGL scene has to be drawn.
//
void CDemoApp::DrawScene()
{
	// TODO: Replace the following sample code with your code to draw the scene.

	if ( !m_pRenderSpline.isNull() && m_bIsShowSpline )
		m_pRenderSpline->Render();

	if ( !m_pRenderModel.isNull() )
	{
	glPushMatrix();
		m_pRenderModel->Render();
	glPopMatrix();
	}

	//extrude
	if ( !m_pExtruder.isNull() )
	{
	glPushMatrix();
		/*glTranslatef(0.0f, 10.0f, 0.0f);*/
		m_pExtruder->Render();
	glPopMatrix();
	}
	
	//loft
	if ( !m_pLofter.isNull() )
	{
	glPushMatrix();
		m_pLofter->Render();
	glPopMatrix();
	}

	//twist
	if ( !m_pTwistApplyer.isNull() )
	{
		m_pTwistApplyer->SetTime( m_Timer.getElapsedTimeInMilliSec()*0.001f );
		(*m_pTwistApplyer)( *m_pTwistCylinder );

		m_pTwistCylinder->SetupBBox();
		
		glPushMatrix();
			glTranslatef(-10.0f, 0.0f, 0.0f);
			m_pTwistCylinder->Render();
		glPopMatrix();
	}

	//wave
	if ( !m_pWaveApplyer.isNull() )
	{
		(*m_pWaveApplyer)( *m_pWaveTorus, m_Timer.getElapsedTime() );

		m_pWaveTorus->SetupBBox();

		glPushMatrix();
			glTranslatef(10.0f, 0.0f, 0.0f);
			m_pWaveTorus->Render();
		glPopMatrix();
	}

	//axis...
	if (m_vModels.size())
	{
	glPushMatrix();
		for (size_t i(0); i<m_vModels.size(); i++)
		{
			m_vModels[i]->Render();
		}
	glPopMatrix();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//4/2/2010 RYF
void CDemoApp::SetupTexture()
{
	AUX_RGBImageRec *pTextureImage = auxDIBImageLoad( ".\\earth.bmp" );

	if( pTextureImage != NULL )
	{
		glGenTextures( 1, &m_iTexId );

		glBindTexture( GL_TEXTURE_2D, m_iTexId );

		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D( GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
			GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data );
	}

	if( pTextureImage )
	{
		if( pTextureImage->data )
			free( pTextureImage->data );

		free( pTextureImage );
	}
}

//半径：1.0f
//高度：4.0f
//圆分段：32
//高度分段：5
void CDemoApp::CreateCylinder()
{
	m_pRenderModel = new CCylinder(1.0, 4.0f, 32, 5);
	m_pRenderModel->Setup();
	m_pRenderModel->DisableTexture();
}

//半径：1.5f
//圆分段：32
void CDemoApp::CreateSphere()
{
	m_pRenderModel = new CSphere(1.5f, 32);
	m_pRenderModel->Setup();
}

//内半径：3.0f
//外半径：0.5f
//圆分段：32
void CDemoApp::CreateTorus()
{
	m_pRenderModel = new CTorus(3.0f, 0.5f, 32);
	m_pRenderModel->Setup();
}

//半径：4.0f
//高度：2.0f
void CDemoApp::CreateCone()
{
	m_pRenderModel = new CCone(4.0f, 2.0f);
	m_pRenderModel->Setup();
}

//生成bspline
void CDemoApp::CreateFlowerSpline()
{
	m_pRenderSpline = new CBspline;
	RVec3 tmpPnt;
	int fl=0;

	//向日葵
	//for (double i=0.0;i<2.0*3.14159265;i+=0.13, fl++)
	//{
	//	tmpPnt.setValue( ((double)(fl%3+2))*cos(i), ((double)(fl%3+2))*sin(i), 0.0f );
	//	m_pRenderSpline->AddKnot(tmpPnt);
	//}

	const float pi = 3.1415926f;
	const float radius = 2.0f;
	for(int i=0; i<=5; ++i)
	{
		float a = (float)i/5.0f*pi*2.0f + pi/2.0f;
		m_pRenderSpline->AddKnot(RVec3(cos(a)*radius, sin(a)*radius,0));
	}

	m_pRenderSpline->Setup();
}

void CDemoApp::CreateDeWaveSpline()
{
	m_pRenderSpline = new CBspline;
	RVec3 tmpPnt;
	int fl=0;
	
	//衰减波
	for (double i=0.0; i<10.0; i+=1.0, fl++)
	{
		tmpPnt.setValue( i, (fl%2)?-i/2.0:i/2.0, 0.0f);
		m_pRenderSpline->AddKnot(tmpPnt);	
	}

	m_pRenderSpline->Setup();
}

void CDemoApp::CreateSeaStarSpline()
{
	m_pRenderSpline = new CBspline;
	RVec3 tmpPnt;
	int fl=0;

	//海星
	for (double i=0.0; i<2.0*3.14159265; i+=0.4, fl++)
	{
		tmpPnt.setValue( ((double)(fl%3+2))*cos(i), ((double)(fl%3+2))*sin(i), 0.0f);
		m_pRenderSpline->AddKnot(tmpPnt);
	}

	m_pRenderSpline->Setup();
}

void CDemoApp::CreateTwistEffect()
{
	m_pTwistCylinder = new CCylinder(1.0, 4.0f, 4, 5);
	m_pTwistCylinder->Setup();

	m_pTwistApplyer = new TwistApplyer;
	m_pTwistApplyer->SetHeight(4.0f);
}

void CDemoApp::CreateWaveEffect()
{
	m_pWaveTorus = new CTorus(3.0f, 0.5f, 32);
	m_pWaveTorus->Setup();

	m_pWaveApplyer = new WaveApplyer;
}

//挤出体
void CDemoApp::CreateExtrudeModel()
{
	//挤出体的profile必须是xoz平面

	//将spline绕X轴旋转得到xoz平面的shape

	//注意顺序
	//旋转没有问题，但是旋转会导致顶点顺序反，最终模型顶点法线也不对了。

	//RMat4 rotMat;
	//rotMat.setRotate(  RQuat4(RVec3(1.0f, 0.0f, 0.0f), 10.0f) );
	//
	//RVec3Array	xozProfile;
	//xozProfile.resize( m_pRenderSpline->GetSize() );

	//for (size_t i=0; i<m_pRenderSpline->GetSize(); i++)
	//{
	//	RVec3 tmpV;
	//	rotMat.multVecMatrix(m_pRenderSpline->GetPnt(i), tmpV);
	//	xozProfile.at(i) = tmpV;
	//}
	//旋转以后的profile为什么不行

	CCircle xozCircle(0.0f, 0.0f, 0.0f, 2.0f);
	xozCircle.Setup();

	m_pExtruder = new CExtrude;
	m_pExtruder->SetProfile( xozCircle.GetProfile() );
	m_pExtruder->SetHeight(10.0f);
	m_pExtruder->Setup();
}

void CDemoApp::CreateLoftModel()
{
	m_pLofter = new CLoft;

	//RVec3Array shape;
	//CCircle circle1(0.0f, 0.0f, 0.0f, 0.5f, 20, XOY);
	//circle1.Setup();
	//shape = circle1.GetProfile();
	////m_pLofter->SetShape( circle1.GetProfile() );

	//m_pRenderCCurve is the loft shape
	RVec3Array newShape;
	newShape.resize( m_pRenderSpline->GetSize() );

	RMat4 scaleMat;
	scaleMat.setScale( RVec3(0.3f, 0.3f, 0.3f) );
	for (size_t i=0; i<m_pRenderSpline->GetSize(); i++)
	{
		RVec3 tmpV;
		scaleMat.multVecMatrix(m_pRenderSpline->GetPnt(i), tmpV);
		newShape.at(i) = tmpV;
	}
	
	m_pLofter->SetShape( newShape );
	//m_pLofter->SetShape( m_pRenderSpline->GetPnts() );

	Real pathRadius = 10.0;
	int pathSegMent = 20;
	int pathBeg = 10;
	RVec3Array path;
	path.resize( pathSegMent-pathBeg+1 );

	for (int i=pathBeg; i<=pathSegMent; i++)
	{
		float cx = pathRadius * cos( (2*M_PI*i)/pathSegMent );
		float cz = pathRadius * sin( (2*M_PI*i)/pathSegMent );
		path.at(i-pathBeg) = RVec3(cx, 0.0f, cz);
	}
	m_pLofter->SetPath( path );
	m_pLofter->Setup();
}

//创建坐标系
void CDemoApp::CreateCompoundAxis()
{
	m_pRenderModel = 0;		//delete the smart pointer.

	//默认位置都是y方向
	Real height = 4.0f;
	
	ModelPtr cone = new CCone(0.5f, 0.5f);
	cone->Setup();

	ModelPtr cylinder = new CCylinder(0.25f, height, 20, 2);
	cylinder->Setup();

	//////////////////////////////////////////////////////////////////////////
	//Y
	//////////////////////////////////////////////////////////////////////////
	RMat4 transY;
	transY.setTranslate( RVec3(0.0f, height/2.0f, 0.0f) );
	ModelPtr yCylinder = ProduceModel<CModel>::transformModel(transY, cylinder);
	m_vModels.push_back( yCylinder );
	
	transY.setTranslate( RVec3(0.0f, height, 0.0f) );
	ModelPtr yCone = ProduceModel<CModel>::transformModel(transY, cone);
	m_vModels.push_back( yCone );

	//////////////////////////////////////////////////////////////////////////
	//X
	//旋转有问题，估计是选择中心点的问题
	//////////////////////////////////////////////////////////////////////////
	RMat4 rotXMatx;
	rotXMatx.setRotate( RQuat4( RVec3(0.0f, 0.0f, 1.0f), -90.0f) );
	ModelPtr xCone = ProduceModel<CModel>::transformModel(rotXMatx, cone);
	m_vModels.push_back( xCone );
}