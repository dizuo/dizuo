#include "ImageList.h"

CImageList::CImageList() : m_lstImages()
{
}

CImageList::CImageList(const CImageList& other)
{
	TImageList::const_iterator iter = other.m_lstImages.begin();
	for (iter; iter!=other.m_lstImages.end(); iter++)
	{
		m_lstImages.push_back(*iter);
	}
	m_iterCurrentImg = m_lstImages.begin();
}

CImageList::~CImageList()
{
}

CImageList& CImageList::operator= (const CImageList& other)
{
	Clear();

	TImageList::const_iterator iter = other.m_lstImages.begin();
	for (iter; iter!=other.m_lstImages.end(); iter++)
	{
		m_lstImages.push_back(*iter);
	}
	m_iterCurrentImg = m_lstImages.begin();
	return *this;
}

void CImageList::Clear()
{
	m_lstImages.clear();
	m_iterCurrentImg = m_lstImages.end();
}

void CImageList::AppendImage(TImagePtr pImage)
{
	m_lstImages.push_back(pImage);
	m_iterCurrentImg = m_lstImages.begin();
}

unsigned CImageList::GetImagesCount() const
{
	return (unsigned)m_lstImages.size();
}

void CImageList::GoToFirstImage()
{
	m_iterCurrentImg = m_lstImages.begin();
}

TImagePtr CImageList::GetCurrentImage() const
{
	TImagePtr pToReturn = NULL;
	if (m_iterCurrentImg != m_lstImages.end() )
		pToReturn = *m_iterCurrentImg;
	return pToReturn;
}

bool CImageList::GoToNextImage()
{
	if (m_iterCurrentImg != m_lstImages.end() )
		m_iterCurrentImg++;
	else
		return false;

	if (m_iterCurrentImg == m_lstImages.end() )
	{
		m_iterCurrentImg = m_lstImages.begin();
		return true;
	}
	return false;
}
