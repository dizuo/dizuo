#ifndef __GROUND_H
#define __GROUND_H

#include	"Object.h"
#include	"Texture.h"
#include	"Config.h"

#define MapSize 500
class Ground : public Object
{
public:
	Ground()
		:m_uTexId(0)
		,width(100)
		,length(100)
	{
		Active();
		m_Type = GROUND;

		m_vPos.setValue(0.0, -5.0, 0.0);
	}
	~Ground(){}
	
	virtual bool Init(){
		width = length = 500;
		m_uTexId = LoadTexture( "Data/ground/SAND0.bmp" );
		return true;
	}
	virtual void Draw(){
		// Draw the ground
		glPushMatrix();
		{
			glBindTexture( GL_TEXTURE_2D, m_uTexId );
			glBegin(GL_QUADS);
			glNormal3f( 0.0f, 1.0f, 0.0f );		glTexCoord2f(0.0f, 0.0f);		glVertex3f( -width, 0.0, -length );
			glNormal3f( 0.0f, 1.0f, 0.0f );		glTexCoord2f(100.0f, 0.0f);		glVertex3f( width,  0.0, -length );
			glNormal3f( 0.0f, 1.0f, 0.0f );		glTexCoord2f(100.0f, 100.0f);	glVertex3f( width,  0.0, length );
			glNormal3f( 0.0f, 1.0f, 0.0f );		glTexCoord2f(0.0f, 100.0f);		glVertex3f( -width, 0.0, length );
			glEnd();
		}glPopMatrix();
	}
private:
	unsigned int m_uTexId;
	float width, length;
};

#endif