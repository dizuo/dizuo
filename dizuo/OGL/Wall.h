#ifndef __WALL_H
#define __WALL_H

#include	"Object.h"
#include	"Texture.h"

class Wall : public Object
{
public:
	enum WallPart{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};

	Wall()
		:m_uCurId(0)
	{
		m_Type = WALL;
		Active();
	}
	~Wall(){}

	virtual bool Init()
	{
		m_uCurId = LoadTexture( "Data/wall.bmp" );
		SetWall(100, 100, 100);

		return true;
	}
	virtual void Draw()
	{
		glBindTexture(GL_TEXTURE_2D, m_uCurId);
		glBegin(GL_QUADS);
		
		// Back	
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() + m_vOffset.x(),	m_vPos.y() - m_vOffset.y(),	m_vPos.z() - m_vOffset.z());
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() + m_vOffset.x(),	m_vPos.y() + m_vOffset.y(),	m_vPos.z() - m_vOffset.z()); 
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() + m_vOffset.y(),	m_vPos.z() - m_vOffset.z());
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() - m_vOffset.y(),	m_vPos.z() - m_vOffset.z());

		// Front
		glNormal3f( 0.0f, 0.0f, -1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() - m_vOffset.y(),	m_vPos.z() + m_vOffset.z());
		glNormal3f( 0.0f, 0.0f, -1.0f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() + m_vOffset.y(),	m_vPos.z() + m_vOffset.z());
		glNormal3f( 0.0f, 0.0f, -1.0f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() + m_vOffset.x(),	m_vPos.y() + m_vOffset.y(),	m_vPos.z() + m_vOffset.z()); 
		glNormal3f( 0.0f, 0.0f, -1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() + m_vOffset.x(),	m_vPos.y() - m_vOffset.y(),	m_vPos.z() + m_vOffset.z());

		// Left	
		glNormal3f( 1.0f, 0.0f, 0.0f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() + m_vOffset.y(),	m_vPos.z() - m_vOffset.z());	
		glNormal3f( 1.0f, 0.0f, 0.0f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() + m_vOffset.y(),	m_vPos.z() + m_vOffset.z()); 
		glNormal3f( 1.0f, 0.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() - m_vOffset.y(),	m_vPos.z() + m_vOffset.z());
		glNormal3f( 1.0f, 0.0f, 0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() - m_vOffset.x(),	m_vPos.y() - m_vOffset.y(),	m_vPos.z() - m_vOffset.z());		

		// Right

		glNormal3f( -1.0f, 0.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() + m_vOffset.x(), m_vPos.y() - m_vOffset.y(),	m_vPos.z() - m_vOffset.z());
		glNormal3f( -1.0f, 0.0f, 0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() + m_vOffset.x(), m_vPos.y() - m_vOffset.y(),	m_vPos.z() + m_vOffset.z());
		glNormal3f( -1.0f, 0.0f, 0.0f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() + m_vOffset.x(), m_vPos.y() + m_vOffset.y(),	m_vPos.z() + m_vOffset.z()); 
		glNormal3f( -1.0f, 0.0f, 0.0f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() + m_vOffset.x(), m_vPos.y() + m_vOffset.y(),	m_vPos.z() - m_vOffset.z());

		glEnd();
	}
	void SetWall(float wid, float hei, float len){
		m_fWidth = wid;	m_fHeight = hei;	m_fLength = len;
		m_vOffset.setValue(wid/2, hei/2, len/2);
	}
private:
	// 距离中心的偏移量: 分别为墙的长，宽，高的各一半.
	gtl::Vec3f	m_vOffset;
	float		m_fWidth;
	float		m_fHeight;
	float		m_fLength;

	unsigned int m_uCurId;
};
#endif