#include	"Font.h"
#include	<string.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<gl/glut.h>

Font::Font()
{
	m_Type = FONT;
	Active();

	m_iFontType = (int)GLUT_BITMAP_9_BY_15;
	//m_iFontType = (int)GLUT_BITMAP_8_BY_13;
	
	FontTypeMap[std::string("8 by 13")] = (int)(GLUT_BITMAP_8_BY_13);
	FontTypeMap[std::string("9 by 15")] = (int)(GLUT_BITMAP_9_BY_15);
	FontTypeMap[std::string("Times Roman 10")]	= (int)(GLUT_BITMAP_TIMES_ROMAN_10);
	FontTypeMap[std::string("Times Roman 24")]	= (int)(GLUT_BITMAP_TIMES_ROMAN_24);
	FontTypeMap[std::string("Helvetica 10")] = (int)(GLUT_BITMAP_HELVETICA_10);
	FontTypeMap[std::string("Helvetica 12")] = (int)(GLUT_BITMAP_HELVETICA_12);
	FontTypeMap[std::string("Helvetica 18")] = (int)(GLUT_BITMAP_HELVETICA_18);
}
Font::~Font()
{
	FontTypeMap.clear();
	m_PosVec.clear();
	m_StrVec.clear();
}

void Font::RenderFont(float x, float y, float z, char* str)
{
	char *ch;
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 127.0/255.0, 0.0);
	glRasterPos3f(x, y, z);
	for (ch=str; *ch != '\0'; ch++) {
		glutBitmapCharacter( (void*)m_iFontType, *ch);
	}
	glColor3f(1.0, 1.0, 1.0);
	glPopAttrib();
}

void Font::RenderFont(float x, float y, char* str)
{
	char *ch;
	glRasterPos2f(x, y);
	for (ch=str; *ch != '\0'; ch++) {
		glutBitmapCharacter( (void*)m_iFontType, *ch);
	}
}

void Font::AddFontElem(float x, float y, float z, char* str)
{
	m_PosVec.push_back( gtl::Vec3f(x, y, z) );
	m_StrVec.push_back( str );
}

void Font::Draw()
{
	assert(m_PosVec.size() == m_StrVec.size());

	for (size_t i=0; i<m_PosVec.size(); i++)
	{
		RenderFont(m_PosVec[i].x(), m_PosVec[i].y(), m_PosVec[i].z(), m_StrVec[i]);
	}
}