#include "Keyboard.h"

#include "util/Helper.h"
#include "Share.h"

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
using namespace std;


//增加打开文件选项

void KeyboardManger::OnKeyHandle(unsigned char key, int x, int y)
{
	static bool bRenderMode = true;
	if( key==27 || key=='q' || key=='Q' )
		Share::setRunFlag(false);
		
	if( key=='H' || key=='h' )
	{
		Share::getHelpInfo()->isShow() ? Share::getHelpInfo()->deactive() : Share::getHelpInfo()->active();
		Share::getFigure()->font_container->IsShow() ? Share::getFigure()->font_container->Deactive() : Share::getFigure()->font_container->Active();
	}
	
	if( key=='C' || key=='c' )
		Share::getFigure()->set_draw_mode( &(Helper::mySolidCylinder) );
		
	if( key=='P' || key=='p' )
		Share::getFigure()->set_draw_mode( &(Helper::mySolidPyramid) );

	if( key=='N' || key=='n' )
		Share::getFigure()->set_draw_mode( &(Helper::myDrawNone) );

	if( key=='M' || key=='m' )
	{
		if(!bRenderMode) 				
		{
			// Render the triangles in fill mode		
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
		}
		else 
		{
			// Render the triangles in wire frame mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
		}
		bRenderMode = !bRenderMode;
	}

	if( key=='L' || key=='l' )
	{
 		BOOL  ret;
 		OPENFILENAME	open_file;
 		const int		file_name_len = 256;
 		wchar_t			file_name[ file_name_len ] = L"";
		std::wstring		wName;
 
 		memset( &open_file, 0, sizeof(OPENFILENAME) );
 		open_file.lStructSize = sizeof(OPENFILENAME);
 		open_file.hwndOwner = NULL;
 		open_file.lpstrFilter = _T("BVH Motion Data (*.bvh)\0*.bvh\0All (*.*)\0*.*\0");
 		open_file.nFilterIndex = 1;
 		open_file.lpstrFile = (LPWSTR)file_name;		//
 		open_file.nMaxFile = file_name_len;
 		open_file.lpstrTitle = _T("Select a BVH file");
 		open_file.lpstrDefExt = _T("bvh");
 		open_file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
 
 		ret = GetOpenFileName( &open_file );
		wName = file_name;
		fileName = Helper::ws2s( wName );	//unicode转化为多字节
		//使用相对路径
		size_t delPos = fileName.find( std::string("data") );
		// 也删掉data
		fileName.erase( fileName.begin(), fileName.begin() + delPos + 4 );	
		//fileName = "data/KICK R.bvh"
		fileName[4] = '/';
		//fileName = "data/KICK R.bvh"
		if( ret )
 		{
 			//Figure的一系列初始化		....1
			//		这个为什么都不可以的啊？？？？？？？？？？？？
			//Share::setFigure ( new Figure("data/KICK R.bvh") );
 			Share::setFigure ( new Figure( "avatar_land.bvh" ) );
 			Share::getFigure()->set_font_pos( GUISystem::GetInstance()->m_pInterface->GetRight().m_Left,
 				GUISystem::GetInstance()->m_pInterface->GetRight().m_Top + 35 );
 			//g_Figure.process_file();	....2
 			Share::getFigure()->recurs_read();
 			Share::getFigure()->pre_recurs_print( Share::getFigure()->get_root(), 1 );
 			Share::getFigure()->process_font( Share::getFigure()->get_root(), 1);
 			Share::setFrameTimeMs( Share::getFigure()->get_frame_time()*1000 );
			
			//激活层次树
			Share::getFigure()->font_container->Active();
				
 			//根据Figure的frame number重新设置进度条的步长		....3
 			GUISystem::GetInstance()->m_pBar->setStepSize( 1024.0f/Share::getFigure()->get_frame_num() );
 		}
	}

	if( key=='S' || key=='s' )
	{
		//Save the file...
		int mYear(0);
		int mMonth(0);
		int mDayOfMonth(0);
		int mHour(0);
		int mMinute(0);
		int mSecond(0);

#if defined(_WIN32)
		SYSTEMTIME local_time;
		GetLocalTime(&local_time);
		mYear = local_time.wYear;
		mMonth = local_time.wMonth;
		mDayOfMonth = local_time.wDay;
		mHour = local_time.wHour;
		mMinute = local_time.wMinute;
		mSecond = local_time.wSecond;
#endif
		std::string fileName("bvh");
		fileName += "_";

		fileName += Helper::Value2Str( mYear );
		fileName += "_";

		fileName += Helper::Value2Str( mMonth );
		fileName += "_";

		fileName += Helper::Value2Str( mDayOfMonth );
		fileName += "_";

		fileName += Helper::Value2Str( mHour );
		fileName += "_";

		fileName += Helper::Value2Str( mMinute );
		fileName += "_";
	
		fileName += Helper::Value2Str( mSecond );
		fileName += "_";

		fileName += ".bvh";

		ofstream outfile ( fileName.c_str() );
		Share::getFigure()->recurs_write( outfile );
		outfile.close();
	}
}

void KeyboardManger::OnSpecialHandle(int key, int x, int y)
{
}

//	打开文件使用了OPENFILENAME结构以后，软件工作的相对目录就变化了，，，
//	不再是工程的当前目录，而是打开文件选择的目录。。。