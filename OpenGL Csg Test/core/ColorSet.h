
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef COLORSET_H
#define COLORSET_H

#include <vector>

class Color;

class ColorSet
{
public:
	ColorSet();
	ColorSet(int nMaxSize);
	virtual ~ColorSet();

	//int GetMaxSize();
	int GetSize();
	Color GetColor(int i);

	void SetColor(int i, const Color & vColor);
	void AddColor(const Color & vColor);

	Color & operator[](int index);

	int length();

	typedef std::vector<Color> ColorVector;

	ColorVector m_pColors;

//private:
//
//	Color * m_pColors;
//	int m_nMaxSize;
//	int m_nSize;
};

#endif // COLORSET_H

