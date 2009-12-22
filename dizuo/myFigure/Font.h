#ifndef __FONT_H
#define __FONT_H

#include "util/Helper.h"
#include "util/gtl/vec3.hpp"
#include "util/Rectangle.hpp"
#include <gl/glut.h>

class Font
{
public:
	Font(int _x, int _y, int _z, int type, const char* str)
		: m_str(str)
		, m_bIsActive(false)
	{
		m_vPos.setValue(_x, _y, _z);
		m_iType = type;

		m_Rect.SetRect( m_vPos.x(), m_vPos.y(), 
						10*strlen(str)+m_vPos.x(), m_vPos.y()-18 );
	}
	~Font(){}

	void Render2()
	{
		glRasterPos2i(m_vPos.x(), m_vPos.y());
		for (size_t i(0); i<m_str.size(); i++)
		{
			glutBitmapCharacter( (void*)m_iType, m_str[i]);
		}
	}
	void Render3()
	{
		Helper::renderRectangle( m_Rect );

		if(m_bIsActive)	//change select font color
			glColor3f((float)247/255.0, (float)171/255.0, (float)0/255.0);
		
		glRasterPos3i(m_vPos.x(), m_vPos.y(), m_vPos.z());
		for (size_t i(0); i<m_str.size(); i++)
		{
			glutBitmapCharacter( (void*)m_iType, m_str[i]);
		}
		
		if(m_bIsActive) //do not effect the following fonts' color
			glColor3f(1.0, 1.0, 1.0);
	}
	void SetType(int type){m_iType = type;}
	void SetPos(int x, int y, int z){ 
		m_vPos.setValue(x, y, z);
	}
	void SetStr(const std::string& str){
		m_str = (str);

		m_Rect.SetRect( m_vPos.x(), m_vPos.y(), 
						10*strlen( m_str.c_str() ) + m_vPos.x(), m_vPos.y() - 18 );
	}
	const char*	GetStr() const { return m_str.c_str(); }
	int		GetPosX()const{return m_vPos.x();}
	int		GetPosY()const{return m_vPos.y();}
	int		GetWid()const {return m_Rect.GetWidth();}
	int		GetHeight()const {return m_Rect.GetHeight();}
	void	Active()	
	{
		m_bIsActive = true;
		m_iType = (int)(GLUT_BITMAP_TIMES_ROMAN_24);
	}
	void	DeActive()	
	{
		m_bIsActive = false;
		m_iType = (int)(GLUT_BITMAP_9_BY_15);
	}
	bool	Check(int x, int y)
	{
		return m_Rect.Intersect(x, y);
	}
private:
	bool		m_bIsActive;
	int			m_iType;
	int			m_iWid;
	int			m_iHeight;
	
	gtl::Vec3i	m_vPos;
	TRectanglei m_Rect;
	std::string	m_str;
};
#endif /* __FONT_H */

/*
int font=(int)GLUT_BITMAP_8_BY_13;
	"8 by 13"			(int)GLUT_BITMAP_8_BY_13);
	"9 by 15"			(int)GLUT_BITMAP_9_BY_15);
	"Times Roman 10"	(int)GLUT_BITMAP_TIMES_ROMAN_10);
	"Times Roman 24"	(int)GLUT_BITMAP_TIMES_ROMAN_24);
	"Helvetica 10"		(int)GLUT_BITMAP_HELVETICA_10);
	"Helvetica 12"		(int)GLUT_BITMAP_HELVETICA_12);
	"Helvetica 18"		(int)GLUT_BITMAP_HELVETICA_18);
*/

/*
不同的字符串   不同的字体
*/

//  [4/11/2009 RenYaFei]
//	重构了Font类，原来的效率可想而知。每次set都要new，delete。。。