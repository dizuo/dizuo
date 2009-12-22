#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "util/Rectangle.hpp"

class Mainframe
{
public:
	Mainframe(int i=0)
		: m_iTexid(i)
	{
		Init();
	}
	~Mainframe()
	{}

	void Render();
	void SetTexid(unsigned i) {m_iTexid = i;}
	bool Init();
	TRectanglei GetBottom()		const{ return m_bottom;};
	TRectanglei GetTop()		const{ return m_top;};
	TRectanglei GetRight()		const{ return m_right;};
	TRectanglei GetMainFrame()	const{ return m_mainframe;};
	TRectanglei GetClient()		const{ return m_client;}
private:
	unsigned m_iTexid;
	//区域
	TRectanglei m_top;	
	TRectanglei m_bottom;
	TRectanglei m_right;
	TRectanglei m_client;		//显示模型区域
	TRectanglei m_mainframe;	//整个框架
};

#endif