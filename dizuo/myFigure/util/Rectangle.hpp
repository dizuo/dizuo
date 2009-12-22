/*************************************************************/
/**					Filename: Rectangle.Hpp		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  3/14/2009		RYF - Creation							**/	
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "gtl/vec2.hpp"

//		left			right	
//	(0,0)--------------------------->X
//	  |					
//	top	----------------> 
//	  |	|		.		|
//	  |	|				|
//bottom<----------------
//	  |	
//	  |
//	  Y
template <class T>
class CRectangle
{
public:
	CRectangle () 
		: m_Top(0), m_Bottom(0)
		, m_Left(0), m_Right(0)   
	{}
	CRectangle (T top, T bottom, T left, T right) 
		: m_Top(top), m_Bottom(bottom)
		, m_Left(left), m_Right(right)
	{}
	CRectangle (const gtl::Vec2<T>& pos, const gtl::Vec2<T>& size)
		: m_Left(pos.x()), m_Top(pos.y())
		, m_Right(pos.x() + size.x()), m_Bottom(pos.y() + size.y())
	{}

	~CRectangle()  
	{}

	// Check if this rectangle intersects with the rectangle 
	// passed as argument. Returns true if that's the case.
	bool Intersect(const CRectangle& rect) const
	{
		return ( (m_Left < rect.m_Right) && (m_Right > rect.m_Left ) &&
				 (m_Bottom > rect.m_Top) && (m_Top < rect.m_Bottom) );
	}

	// check if the point(x, y) is inside the rectangle
	bool Intersect(const T& x, const T& y) const
	{
		return ( (m_Left < x) && (m_Right > x ) &&
				 (m_Top < y) && (m_Bottom > y) );
	}

	bool Contain(const CRectangle& rect) const
	{
		return(	(rect.m_Left > m_Left) && (rect.m_Right < m_Right) &&
				(rect.m_Top > m_Top) && (rect.m_Bottom < m_Bottom) );
	}

	// Returns true if this is a valid rectangle (has a width and
	// a height).
	bool IsValid() const
	{
		if ( (m_Top > m_Bottom) || (m_Left > m_Right) )
			return false;
		return true;
	}

	void OffsetRect(int xOffset, int yOffset)
	{
		m_Top += yOffset;
		m_Bottom += yOffset;
		m_Left += xOffset;
		m_Right += xOffset;
	}

	T		GetWidth()  const { return m_Right - m_Left;  }
	T		GetHeight() const { return m_Bottom - m_Top;  }

	gtl::Vec2<T> GetPos() const { return gtl::Vec2<T>(m_Left, m_Top); }
	gtl::Vec2<T> GetSize() const { return gtl::Vec2<T>(GetWidth(), GetHeight()); }
	gtl::Vec2<T> GetCenter() const { return gtl::Vec2<T>( (m_Left+m_Right)/2, (m_Top+m_Bottom)/2 );}
	
	//x-axis coordinates of the priority
	void SetRect(const T& l, const T& b, const T& r, const T& t )
	{
		m_Top = t; m_Bottom = b; m_Left = l; m_Right = r;
	}

	// The position of the top side of the rectangle
	T  m_Top;
	// The position of the bottom side of the rectangle
	T  m_Bottom;
	// The position of the left side of the rectangle
	T  m_Left;
	// The position of the right side of the rectangle
	T  m_Right;	
};

// Typdefs for the most used types.
typedef CRectangle<int>    TRectanglei;
typedef CRectangle<float>  TRectanglef;
typedef CRectangle<double> TRectangled;


#endif  // _RECTANGLE_H_