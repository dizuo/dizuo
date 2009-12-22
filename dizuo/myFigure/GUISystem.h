/*************************************************************/
/**					Filename: Gui.H				            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  3/14/2009		RYF - Creation							**/
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef GUISYSTEM_H_
#define GUISYSTEM_H_

#include "util/Singleton.hpp"
#include "util/ProgressBar.h"
#include "util/timer.h"

#include "MainFrame.h"
#include "ButtonManger.h"


class Font;

class GUISystem : public Singleton<GUISystem>
{
	friend class Singleton<GUISystem>;
protected:
	GUISystem();
	~GUISystem();

public:
	//绘制
	void Render();

	//创建，初始化
	//坐标统一从左上角开始
	bool SetUp();

	enum { PICNUM = 7 };
	struct ButtonInfo {
		char* path;
		char* name;
	};
	static ButtonInfo m_vButtonInfo[ PICNUM ];

	Font			*m_pFps;
	ProgressBar		*m_pBar;
	Mainframe		*m_pInterface;
};

//	Button专门写一个管理类
//  [5/15/2009 RYF]
//		绝对可以写成一个管理类

//		可以把path，name，pFunc写成一个结构体,
//		按钮，图片，事件 联系起来...
#endif