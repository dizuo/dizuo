/*************************************************************/
/**					Filename: ProgressBar.H		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  3/14/2009		RYF - Creation							**/
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <cassert>
#include "Rectangle.hpp"

//在glOrtho模式下，原点位于左上角
//坐标设置注意...
class ProgressBar
{
public:
	ProgressBar(int bg_x, int bg_y, int length, int width)
		: m_iFrameNum(1000), m_iFigureCurrentFram(0)
	{
		m_fDx = (float)length/(float)m_iFrameNum;

		m_Rect.m_Left = static_cast<float>(bg_x);
		m_Rect.m_Bottom = static_cast<float>(bg_y);
		m_Rect.m_Right = static_cast<float>(bg_x);
		m_Rect.m_Top = m_Rect.m_Bottom - width;

		m_fzValue = 0.2f;
	}
	~ProgressBar(){};
	
	//绘制
	void Render()
	{
		m_Rect.m_Right = m_Rect.m_Left + m_iFigureCurrentFram * m_fDx;
		glPushMatrix();
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		{
			glBegin(GL_QUADS);
			glVertex3f(m_Rect.m_Left, m_Rect.m_Bottom, m_fzValue);
			glVertex3f(m_Rect.m_Right, m_Rect.m_Bottom, m_fzValue);

			glVertex3f(m_Rect.m_Right, m_Rect.m_Top, m_fzValue);
			glVertex3f(m_Rect.m_Left, m_Rect.m_Top, m_fzValue);
			glEnd();
		}glPopAttrib();
		glPopMatrix();
	}
	//更新位置
	void setProgress(int num)
	{
		m_iFigureCurrentFram = num;
	}

	void setStepSize(float step)
	{
		m_fDx = step;
	}

	float getStep() const 
	{
		return m_fDx;
	}
	
	float getProgress() const
	{
		return m_fDx * m_iFigureCurrentFram;
	}
private:
	ProgressBar(){};

	int m_iFigureCurrentFram;
	int m_iFrameNum;
	float m_fDx;
	float m_fzValue;

	TRectanglef m_Rect;
};

#endif