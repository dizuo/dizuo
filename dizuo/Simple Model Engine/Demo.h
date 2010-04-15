#if !defined(DEMO_H_INCLUDED)
#define DEMO_H_INCLUDED

#pragma once

#include "resource.h"
#include "App.h"
#include "SmartPtr.h"

/////////////////////////////////////////////////////////////////////////////
// CDemoApp
// Application class

class CModel;
class CBspline;

class CCylinder;
class TwistApplyer;

class CTorus;
class WaveApplyer;

class CExtrude;
class CLoft;
class CLathe;

typedef CSmartPtr<CModel> ModelPtr;
typedef CSmartPtr<CCylinder> CylinderPtr;

//////////////////////////////////////////////////////////////////////////
//坐标系Y向下为正，照相机设置的问题
//////////////////////////////////////////////////////////////////////////

class CDemoApp : public CApp
{
protected:
	CSmartPtr<CModel>	m_pRenderModel;

	bool				m_bIsShowSpline;
	CSmartPtr<CBspline>	m_pRenderSpline;
	
	//Compound Scene
	std::vector<ModelPtr> m_vModels; 

	//CExtrude
	CSmartPtr<CExtrude>	m_pExtruder;

	//CLoft
	CSmartPtr<CLoft>	m_pLofter;

	//CLathe
	CSmartPtr<CLathe>	m_pLather;

	//Twist
	CSmartPtr<CCylinder> m_pTwistCylinder;
	CSmartPtr<TwistApplyer> m_pTwistApplyer;

	//Wave
	CSmartPtr<CTorus>	m_pWaveTorus;
	CSmartPtr<WaveApplyer> m_pWaveApplyer;

	//texture
	unsigned int	m_iTexId;

	virtual void InitScene();
	virtual void KillScene();
	virtual void DrawScene();

	virtual void PreCreateWindow(WNDCLASSEX &wcex, DWORD &dwStyle, DWORD &dwExStyle, int &x, int &y, int &cx, int &cy);
	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(int nCmdID, int nEvent);

	void SetupTexture();

	void CreateCylinder();
	void CreateSphere();
	void CreateTorus();
	void CreateCone();

	void CreateSeaStarSpline();
	void CreateDeWaveSpline();
	void CreateFlowerSpline();

	void CreateTwistEffect();
	void CreateWaveEffect();

	void CreateExtrudeModel();
 	void CreateLoftModel();
 	void CreateLatheModel();

	void CreateCompoundAxis();
public:
	CDemoApp();
	virtual ~CDemoApp();
};


#endif // !defined(DEMO_H_INCLUDED)
