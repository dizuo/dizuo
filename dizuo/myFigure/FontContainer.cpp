#include "FontContainer.h"

#include "util/global.h"


// ----------------------------------------------------------------
//	Name：			FontContainer::FontContainer
//	Description:	构造函数初始化...
//	Return Value:	
// ----------------------------------------------------------------
FontContainer::FontContainer()
	: m_bIsSkip(true)
	, m_sSelectName("")
	, m_iLast(-1)
	, m_bIsShow(false)
{
	FontTypeMap[std::string("8 by 13")] = (int)(GLUT_BITMAP_8_BY_13);
	FontTypeMap[std::string("9 by 15")] = (int)(GLUT_BITMAP_9_BY_15);
	FontTypeMap[std::string("Times Roman 10")]	= (int)(GLUT_BITMAP_TIMES_ROMAN_10);
	FontTypeMap[std::string("Times Roman 24")]	= (int)(GLUT_BITMAP_TIMES_ROMAN_24);
	FontTypeMap[std::string("Helvetica 10")] = (int)(GLUT_BITMAP_HELVETICA_10);
	FontTypeMap[std::string("Helvetica 12")] = (int)(GLUT_BITMAP_HELVETICA_12);
	FontTypeMap[std::string("Helvetica 18")] = (int)(GLUT_BITMAP_HELVETICA_18);
}

// ----------------------------------------------------------------
//	Name：			FontContainer::~FontContainer
//	Description:	析构函数
//	Return Value:	
// ----------------------------------------------------------------
FontContainer::~FontContainer()
{
	FontTypeMap.clear();

	for (size_t i=0; i<m_vFontVec.size(); i++)
		SAFE_DELETE(m_vFontVec[i]);
	
	m_vFontVec.clear();
}

// ----------------------------------------------------------------
//	Name：			FontContainer::AddFontElem
//	Description:	位置x,y,z; 字体类型string; 内容str
//	Return Value:	void
// ----------------------------------------------------------------
void FontContainer::AddFontElem( int x, int y, int z, 
								 std::string FontType, const char *str )
{
	Font*  pTem = new Font(x, y, z, FontTypeMap[FontType], str);
	m_vFontVec.push_back( pTem );	
}

// ----------------------------------------------------------------
//	Name：			FontContainer::Render
//	Description:	逐一渲染字体
//	Return Value:	void
// ----------------------------------------------------------------
void FontContainer::Render()
{
	if( m_bIsShow==false )
		return;

	glColor4f(1.0, 1.0, 1.0, 1.0);
	for (size_t i=0; i<m_vFontVec.size(); i++)
	{
		m_vFontVec[i]->Render3();
	}
}

// ----------------------------------------------------------------
//	Name：			FontContainer::OnMouseDown
//	Description:	鼠标位置x,y，检测鼠标是否选中了某个font，
//					将选中的字体类型更改为:Times Roman 24
//					Font的位置Pos是左下角，而坐标系是左上角，y轴向下
//	Return Value:	bool
// ----------------------------------------------------------------
bool FontContainer::OnMouseDown(int mousex, int mousey)
{
	for (size_t i=0; i<m_vFontVec.size(); i++)
	{
		if( m_vFontVec[i]->Check(mousex, mousey) )
		{
			if( m_sSelectName != "" 
				&& i != m_iLast)
				m_bIsSkip = true;

			if( m_bIsSkip )
			{
				if(m_iLast != -1)	//不是第一次
					m_vFontVec[m_iLast]->DeActive();
				m_vFontVec[i]->Active();
				m_sSelectName = std::string( m_vFontVec[i]->GetStr() );
				m_iLast = i;
				
				m_bIsSkip = false;
			}
			else
			{
				m_vFontVec[i]->DeActive();
				m_sSelectName = "";
				m_bIsSkip = true;
				return false;
			}
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------
//	Name：			FontContainer::OnMouseUp
//	Description:	在按键起时，须检测是否有font被选中，
//					如果当前没有font被选择，必须确保所有字体类型都是
//					初始类型，特别是之前被选中的font
//	Return Value:	
// ----------------------------------------------------------------
void FontContainer::OnMouseUp()
{
	//每次up的时候检测
}

//主要是逻辑设计......
//  [3/17/2009 RenYaFei]
//		font选择逻辑控制OK了...
//		设置颜色，
