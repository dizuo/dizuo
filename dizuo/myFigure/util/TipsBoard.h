/*************************************************************/
/**					Filename: TipsBoard.H		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  4/23/2009 	RYF - Creation							**/
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef TIPSBOARD_H_
#define TIPSBOARD_H_

#include "gtl/vec2.hpp"

class TipsBoard
{
public:
	TipsBoard(int wid, int height) { init(wid, height); };
	~TipsBoard(){};

	bool isShow() const { return _isShow; };
	void active() { _isShow = true; };
	void deactive() { _isShow = false; };
	void showInfo();

	void setPos( const gtl::Vec2f& _pos) { pos = _pos; }
	enum { BUFFERSIZE = 16 };
private:
	void		init(int, int);
	bool		_isShow;
	gtl::Vec2f  pos;
	gtl::Vec2f  size;
	gtl::Vec2f	offset;
	int			fontType;

	static char* FontList[BUFFERSIZE];
};

//////////////////////////////////////////////////////////////////////////
//	不要过度依赖于Font类
//	这个类本来就不太好，将来需重构
#endif