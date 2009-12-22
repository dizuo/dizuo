#ifndef __CBUTTON_H_
#define __CBUTTON_H_

#include "CTexture.h"
#include "global.h"
#include <vector>
#include <fstream>

class CButton
{
public:
	CButton(){};
	CButton(int x, int y, int Width, int Height);
	~CButton();
	
	//虚函数 不可以是模板函数
	void Render();
	virtual bool OnMouseDown(LPARAM lParam);
	virtual void OnMouseUp(LPARAM lParam);
	
	virtual bool OnMouseDown(int MouseX, int MouseY);
	virtual void OnMouseUp();
	
	void	SetTexId(int id){ m_iTexID = id; }
	GLuint	GetTexId()const { return m_iTexID; };
	void	SetVisible(){ m_bVisible = true;};
	void	SetInVislble(){m_bVisible = false;}

	void	SetPos(int x, int y){ m_ix=x, m_iy=y; };
protected:
	bool m_bVisible;
	float m_iZOrder;
	float m_fAlpha;
	bool m_bPressed;

	GLuint m_iTexID;
	int m_ix;
	int m_iy;
	int m_iWidth;
	int m_iHeight;
};

//Just like "PAUSE" button: Firstly you hit it, it means pause the play,
//but when you hit it secondly it means continue the play.
class CButtonTwoHit : public CButton
{
public:
	CButtonTwoHit(int x, int y, int width, int height)
		:CButton(x, y, width, height)
		, m_bIsFirst(true)
	{
	}

	void Active(){ m_fAlpha = 0.3f; };
	void DeActive(){ m_fAlpha = 1.0f; };

	bool IsDown() {return m_bIsFirst;}
	bool OnMouseDown(LPARAM lParam)
	{
 		if ( CButton::OnMouseDown(lParam) )
		{
			if( m_bIsFirst )	//第一次按下:透明度降为0.3
				Active();
			else
				DeActive();		//第二次按下：回复为1.0
			
			m_bIsFirst = !m_bIsFirst;
			
			return true;
		}
		return false;
	}
private:
	bool m_bIsFirst;
};


//This Button is movable 
class CButtonMovable : public CButton
{
public:
	CButtonMovable(int x, int y, int width, int height)
		: CButton(x, y, width, height)
		, m_bIsDragging(false)
	{
		m_vOffset.x = 0;  m_vOffset.y = 0;
	}
	~CButtonMovable(){};

	// 鼠标主要移动 就会调用OnMouseMove函数
	void OnMouseMove(int x, int y)
	{
		if( m_bIsDragging )
		{
			// m_vStartPos为移动过程中的鼠标位置相对于按钮左下角的偏移量,里面的数值为负...
			m_ix = m_vOffset.x + x;
			m_iy = m_vOffset.y + y;
		}
	}
	bool OnMouseDown(int x, int y)
	{
		if ( CButton::OnMouseDown(x, y) )
		{
			m_bIsDragging = true;
			// 记录下鼠标的起始位置
			m_vOffset.x = m_ix - x;
			m_vOffset.y = m_iy - y;
			return true;
		}
		return false;
	}

	void OnMouseUp()
	{
		CButton::OnMouseUp();
		m_bIsDragging = false;
	}
private:
	//是否移动按钮
	bool m_bIsDragging;
	struct Vec2{int x, y;};

	//在OnMouseDown时候计算出鼠标和按钮的偏移量(m_vMousePos和m_ix,m_iy)
	Vec2 m_vOffset;
};


//////////////////////////////////////////////////////////////////////////
// 我的初衷：
//		一个按钮对应一个图片贴图：格式TGA，32位.
//		每个按钮有坐标，尺寸.
//		对鼠标事件作出响应.			
// [3/3/2009 RenYaFei]
//		实现了两种不同的按钮类型.
// [3/3/2009 RenYaFei]
//		实现了可以移动的按钮，但是几个位置参数，有点晕，
//////////////////////////////////////////////////////////////////////////
//Button里面需要保存一个窗口高度
//	960数字问题
//  因为这里button的所有坐标都是在做坐标系：
//							原点位于左下角
//							Y轴向上，X轴向右
//	正因为这样的前提，付出的代价是CAPP，CButton里面都需要保存一个窗口高度

//has-a 的关系
// 查看鼠标移动的位置
// 			_LOG( std::string("Mouse Down at [") +  toStr<int>(m_vStartPos.x) + std::string(" ")
// 				+ toStr<int>(m_vStartPos.y) + std::string("]") );
// 		_LOG( std::string("Mouse Move by the point: [") +  toStr<int>(m_vMouseMove.x) + std::string(" ")
// 			+ toStr<int>(m_vMouseMove.y) + std::string("]") );
//////////////////////////////////////////////////////////////////////////
//[3/14/2009 RenYaFei]
//	glut中的MouseMove函数和win32中的MouseMove不一样，
//	前者只有鼠标按下的时候才move响应，
//	而win32中的鼠标move则一直处于响应状态.
//	当前版本为glut版本:CButtonMovable中的OnMouseDown，OnMouseMove做了修改
#endif

