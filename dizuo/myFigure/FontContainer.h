/*************************************************************/
/**					Filename: FontContainer.H	            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  3/16/2009		RYF - Creation							**/
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef FONT_CONTAINER_H
#define FONT_CONTAINER_H

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include "util/Font.h"

class FontContainer
{
public:
	FontContainer();
	~FontContainer();

	void Render();
	void AddFontElem(int x, int y, int z, std::string FontType, const char* str);

	bool OnMouseDown(int mousex, int mousey);
	void OnMouseUp();
	std::string GetSelectName() const {return m_sSelectName;}

	bool IsShow() const { return m_bIsShow; }
	void Active() { m_bIsShow = true; }
	void Deactive() {  m_bIsShow = false; }
private:
	std::vector<Font*>			m_vFontVec;	
	bool		m_bIsSkip;	//ÊÇ·ñÒªÇÐ»»Ñ¡Ôñ
	std::string m_sSelectName;
	int			m_iLast;
	bool		m_bIsShow;

	std::map<std::string, int>	FontTypeMap;
};

#endif