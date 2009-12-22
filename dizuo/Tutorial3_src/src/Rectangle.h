#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

// A simple utility template class which contains information for
// an axis-aligned rectangle.
template <class T>
class CRectangle
{
public:
	// Default constructor
	CRectangle () : m_Top(0), m_Bottom(0), m_Left(0), m_Right(0)   { }
	// Constructor which takes the rectangle coordinates as argument
	CRectangle (T top, T bottom, T left, T right) 
		: m_Top(top), m_Bottom(bottom), m_Left(left), m_Right(right)   { }
	~CRectangle()  { }

	// Returns the width of the rectangle
	T GetWidth()  const  { return m_Right - m_Left;  }
	// Returns the height of the rectangle
	T GetHeight() const  { return m_Bottom - m_Top;  }

	// Check if this rectangle intersects with the rectangle 
	// passed as argument. Returns true if that's the case.
	bool Intersect(const CRectangle& rect) const
	{
		return ((m_Left < rect.m_Right) && (m_Right > rect.m_Left ) &&
			    (m_Bottom > rect.m_Top) && (m_Top < rect.m_Bottom) );
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