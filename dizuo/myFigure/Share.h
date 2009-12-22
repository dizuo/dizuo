#ifndef SHARE_H_
#define SHARE_H_

#include "util/Rectangle.hpp"		//gtl.hpp必须位于windows.h前面
#include "util/TipsBoard.h"
#include "util/SmartPtr.h"

#include "GUISystem.h"
#include "Figure.h"
#include "Joint.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "DragCamera.h"
#include "TrackUI.h"

#include <gl/glut.h>

// ----------------------------------------------------------------
//  Name:           class Share
//  Description:    存储所有的全局变量成员
//					和一些全局的define定义.
//					GUISystem.cpp	---->  Share Data
//					Main.cpp		---->  Share Data
//					Mouse.cpp		---->  Share Data
//					KeyBoard.cpp	---->  Share Data
//  Create Time:    [4/5/2009 RenYaFei]
// ----------------------------------------------------------------
class Share
{
	//DATA
	static const GLint	g_iWindowWid = 1024;
	static const GLint	g_iWindowHeigh = 768;
	static bool			g_bKeepRunning;

	static SmartPtr<TrackUI>	g_pTrack;
	static SmartPtr<Figure>		g_pFigure;
	static SmartPtr<DragCamera>	g_pCamera;
	static SmartPtr<TipsBoard>	g_pInfoBoard;
	static Joint*				g_pSelectJoint;

	static int			g_frameTimeMs;
private:
	//NOT IMPLEMENT
	Share(){};

public:

	//MANIPULATORS
	static void setHelpInfo(TipsBoard* info) { g_pInfoBoard = info; }
	static void setCamera(DragCamera* camera) {g_pCamera = camera;}
	static void setTrack(TrackUI* track) {g_pTrack = track;}
	static void setFigure(Figure* figure) { g_pFigure = figure; }
	static void setSelectJoint(Joint* joint) { g_pSelectJoint = joint; }

	static void setFrameTimeMs(int frameTime) { g_frameTimeMs = frameTime; }
	static void setRunFlag(bool flag) {g_bKeepRunning = flag;}
	
	static void destroy();

	//ACCESSOR
	static SmartPtr<TipsBoard> getHelpInfo() { return g_pInfoBoard; }
	static SmartPtr<DragCamera> getCamera() {return g_pCamera;}
	static SmartPtr<TrackUI> getTrack() {return g_pTrack;}
	static SmartPtr<Figure> getFigure() { return g_pFigure; }
	static Joint* getSelectJoint() {  return g_pSelectJoint; }

	static GLint getWindowWid() { return g_iWindowWid; }
	static GLint getWindowHeight() { return g_iWindowHeigh; }
	static int   getFrameTimeMs() { return g_frameTimeMs; }
	static bool  getRunFalg()  { return g_bKeepRunning; }
}; //class Share

//DEFINE
#define BUTTON  ButtonManger::GetInstance()
#define GUI		GUISystem::GetInstance()
#define bRECT	GUISystem::GetInstance()->m_pInterface->GetBottom()
#define rRECT	GUISystem::GetInstance()->m_pInterface->GetRight()
#define cRECT	GUISystem::GetInstance()->m_pInterface->GetClient()

#endif

//////////////////////////////////////////////////////////////////////////
//  [5/6/2009 RYF]
//		指针的set函数接口有点问题，存在内存泄漏
//				1，在复制之前先SAFE_DELETE
//				2，使用智能指针
//		把指针全部改成了SmartPtr类型的
//		g_pSelectJoint只能使用一般的指针，浅拷贝