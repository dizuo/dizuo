#include "Share.h"

bool		Share::g_bKeepRunning = true;
int			Share::g_frameTimeMs = 0;

SmartPtr<TrackUI>		Share::g_pTrack = NULL;
SmartPtr<Figure>		Share::g_pFigure = NULL;
SmartPtr<DragCamera>	Share::g_pCamera = NULL;
SmartPtr<TipsBoard>		Share::g_pInfoBoard = NULL;
Joint*					Share::g_pSelectJoint = NULL;

void Share::destroy()
{
// 	SAFE_DELETE( g_pFigure );
// 	SAFE_DELETE( g_pCamera );
// 	SAFE_DELETE( g_pTrack );
// 	SAFE_DELETE( g_pInfoBoard );

	std::cout << "delete the global data members..." << endl;
}


//////////////////////////////////////////////////////////////////////////
//对于成员变量为数组情况;  修改只能通过set接口实现						//
//修改WatchPoint变量的下面语句错误：									//
//	Share::getWatchPoint().setValue( 0., 0., 50.0 );					//
//	该语句只是对WatchPoint的拷贝做了修改								//
//	因为getWatchPoint传出的只是拷贝，而不是引用，及变量本身.			//
//////////////////////////////////////////////////////////////////////////