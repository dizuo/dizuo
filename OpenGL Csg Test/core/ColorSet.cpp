
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ColorSet.h"

#include "Color.h"

ColorSet::ColorSet()
{
	//m_nMaxSize = 10000;
	//m_nSize = 0;
	//m_pColors = new Color[m_nMaxSize];

	//length = m_nSize;
}

ColorSet::ColorSet(int nMaxSize)
{
	//m_nMaxSize = nMaxSize;
	//m_nSize = 0;
	//m_pColors = new Color[m_nMaxSize];

	//length = m_nSize;
}

ColorSet::~ColorSet()
{
	//delete [] m_pColors;
}

//int ColorSet::GetMaxSize()
//{
//	return m_nMaxSize;
//}

int ColorSet::GetSize()
{
	//return m_nSize;
	return m_pColors.size();
}

int ColorSet::length()
{
	return m_pColors.size();
}

Color ColorSet::GetColor(int i)
{
	if(i < 0) return Color();
	if(i >= m_pColors.size()) return Color();

	return m_pColors[i];
}

void ColorSet::SetColor(int i, const Color & vColor)
{
	if(i < 0) return;
	if(i >= m_pColors.size()) return;

	m_pColors[i] = vColor;
}

void ColorSet::AddColor(const Color & vColor)
{
	m_pColors.push_back(vColor);

	//if(m_nSize >= m_nMaxSize) return;

	//m_pColors[m_nSize] = vColor;
	//m_nSize++;
	//length = m_nSize;
}

Color & ColorSet::operator[](int index)
{
	Color & pColor = m_pColors[index];

	return pColor;
}
