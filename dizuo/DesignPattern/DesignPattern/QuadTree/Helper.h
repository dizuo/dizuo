#ifndef HELPER_H_
#define HELPER_H_

#include<cmath>
#include<vector>
#include "Rectangle.hpp"
#include "Color.h"
#include <gl/freeglut.h>


class Helper
{
public:
	template<class T> static void myAddRange(std::vector<T>& whole, const std::vector<T>& part);
	template<typename T> static int getBufferSize(T& buffer);

	static void renderBitmapString2D( float x, float y, void *font, char *string );
	static void renderBitmapString3D( float x, float y, float z, void* font, std::string str );
	static void renderRectangle( const TRectanglef& rect, Color4 color=Color4(0xFF0000), bool isEmpty = true );
	static void renderElipse( const TRectanglef& rect, Color4 color=Color4(0xFF0000), bool isEmpty = true );
	
	// ----------------------------------------------------------------
	//	Name：			ColorStable
	//	Description:	选择颜色
	// ----------------------------------------------------------------
	static GLuint ColorStable[24];		//加个24用getSize就可以的
};

// ----------------------------------------------------------------
//	Name：			myAddRange
//	Description:	将part数组中元素添加到whole中去
//	Return Value:	void
// ----------------------------------------------------------------
template<class T>
void Helper::myAddRange(std::vector<T>& whole, const std::vector<T>& part)
{
	for (size_t i(0); i< part.size(); i++)
	{	whole.push_back( part[i] ); }
}

// ----------------------------------------------------------------
//	Name：			getBufferSize
//	Description:	通过模板获得数组元素个数
//	Return Value:	int
// ----------------------------------------------------------------
template<typename T>
int Helper::getBufferSize(T& buffer)    
{    
	int n;  
	n=sizeof(buffer)/sizeof(*buffer); 
	return n;  
}

#endif

//////////////////////////////////////////////////////////////////////////
// 模板不支持分离模型
// 静态成员数组 申明定义