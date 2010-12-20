#ifndef _DKGL_RECT2D_H_
#define _DKGL_RECT2D_H_

template <class T>    
class Rect2D
{
public:
	Rect2D(){};
	Rect2D(T _x, T _y, T  _width, T _height) : x(_x), y(_y), width(_width), height(_height){};
	Rect2D(const Rect2D& rc)
	{
		x = rc.x;
		y = rc.y;
		width = rc.width;
		height = rc.height;
	};
	
	T x;
	T y;
	T width;
	T height;
};

typedef Rect2D<int>			intRect2D;
typedef Rect2D<float>		ftRect2D;
typedef Rect2D<double>	dbRect2D;

#endif