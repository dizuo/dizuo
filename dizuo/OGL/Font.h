#ifndef __FONT_H
#define __FONT_H

#include	"gtl/vec3.hpp"
#include	<string>
#include	<map>

#include	"Object.h"

using std::map;
using std::string;

class Font : public Object
{
public:
	Font();
	virtual ~Font();
	
	void RenderFont(float x, float y, float z, char* str);
	void RenderFont(float x, float y, char* str);
	void SetFontType(const std::string& type){
		m_iFontType = FontTypeMap[type];
	}

 	void AddFontElem(float x, float y, float z, char* str);
 	virtual void Draw();
	virtual bool Init(){return true;}
private:
	int		m_iFontType;
	std::map<std::string, int>	FontTypeMap;
	std::vector<gtl::Vec3f>		m_PosVec;
	std::vector<char*>	m_StrVec;
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
