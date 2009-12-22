
#include "Image.h"
#include "Exception.h"
#include "TextureManager.h"

using namespace std;

CImage::CImage(const string& strFileName) 
  : m_pTexture(NULL), m_rectTextCoord()
{
	// This line will throw an exception if the texture is not found. 
	m_pTexture = CTextureManager::GetInstance()->GetTexture(strFileName); 
	m_pTexture->AddReference();

	// Set the texture coordinate to the full texture
	m_rectTextCoord.m_Top = m_rectTextCoord.m_Left = 0;
	m_rectTextCoord.m_Bottom = m_pTexture->GetHeight();
	m_rectTextCoord.m_Right = m_pTexture->GetWidth();
}

CImage::CImage(const string& strFileName, const TRectanglei& textCoord)
  : m_pTexture(NULL), m_rectTextCoord(textCoord)
{
	// This line will throw an exception if the texture is not found. 
	m_pTexture = CTextureManager::GetInstance()->GetTexture(strFileName); 
	m_pTexture->AddReference();
}

TImagePtr CImage::CreateImage(const string& strFileName)
{
	TImagePtr imgPtr(new CImage(strFileName));
	return imgPtr;
}

TImagePtr CImage::CreateImage(const string& strFileName, const TRectanglei& textCoord)
{
	TImagePtr imgPtr(new CImage(strFileName,textCoord));
	return imgPtr;
}


CImage::~CImage()
{
	if (m_pTexture)
		m_pTexture->ReleaseReference();
}

void CImage::BlitImage(int iXOffset, int iYOffset) const
{
	if (m_pTexture)
	{
		m_pTexture->Bind();

		// Get the coordinates of the image in the texture, expressed
		// as a value from 0 to 1.
		float Top	 = ((float)m_rectTextCoord.m_Top)/m_pTexture->GetHeight();
		float Bottom = ((float)m_rectTextCoord.m_Bottom)/m_pTexture->GetHeight();
		float Left   = ((float)m_rectTextCoord.m_Left)/m_pTexture->GetWidth();
		float Right  = ((float)m_rectTextCoord.m_Right)/m_pTexture->GetWidth();

		// Draw the textured rectangle.
		glBegin(GL_QUADS);
		glTexCoord2f(Left,Top);		glVertex3i(iXOffset,iYOffset,0);
		glTexCoord2f(Left,Bottom);	glVertex3i(iXOffset,iYOffset+m_rectTextCoord.GetHeight(),0);
		glTexCoord2f(Right,Bottom);	glVertex3i(iXOffset+m_rectTextCoord.GetWidth(),iYOffset+m_rectTextCoord.GetHeight(),0);
		glTexCoord2f(Right,Top);	glVertex3i(iXOffset+m_rectTextCoord.GetWidth(),iYOffset,0);
		glEnd();
	}
}

void CImage::BlitImagePart(int iXOffset, 
						   int iYOffset, 
						   const TRectanglei& part)
{
	// Check if the part is completely in the image
	assert(part.m_Top>=0);
	assert(part.m_Bottom<=m_rectTextCoord.GetHeight());
	assert(part.m_Left>=0);
	assert(part.m_Right<=m_rectTextCoord.GetWidth());

	if (m_pTexture)
	{
		m_pTexture->Bind();

		TRectanglei newTextRect = part;
		newTextRect.OffsetRect(m_rectTextCoord.m_Left,m_rectTextCoord.m_Top);

		// Get the coordinates of the image in the texture, expressed
		// as a value from 0 to 1.
		float Top	 = ((float)newTextRect.m_Top)/m_pTexture->GetHeight();
		float Bottom = ((float)newTextRect.m_Bottom)/m_pTexture->GetHeight();
		float Left   = ((float)newTextRect.m_Left)/m_pTexture->GetWidth();
		float Right  = ((float)newTextRect.m_Right)/m_pTexture->GetWidth();

		// Draw the textured rectangle.
		glBegin(GL_QUADS);
		glTexCoord2f(Left,Top);		glVertex3i(iXOffset,iYOffset,0);
		glTexCoord2f(Left,Bottom);	glVertex3i(iXOffset,iYOffset+newTextRect.GetHeight(),0);
		glTexCoord2f(Right,Bottom);	glVertex3i(iXOffset+newTextRect.GetWidth(),iYOffset+newTextRect.GetHeight(),0);
		glTexCoord2f(Right,Top);	glVertex3i(iXOffset+newTextRect.GetWidth(),iYOffset,0);
		glEnd();
	}
}

