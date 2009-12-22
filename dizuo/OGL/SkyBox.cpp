// SkyBox.cpp: implementation of the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////

#include "SkyBox.h"
#include <iostream>

#include <gl/glext.h>
#include "Config.h"

//#define GL_CLAMP_TO_EDGE	0x812F						// This is for our skybox textures
enum Index{	BACK_ID, FRONT_ID,	BOTTOM_ID, TOP_ID,	LEFT_ID, RIGHT_ID };

CSkyBox::CSkyBox()
{
	Active();

	m_Type = SKYBOX;
	m_vPos.setValue(0.0, 0.0, 0.0);
}

CSkyBox::~CSkyBox()
{}

void CSkyBox::Draw()
{
	glPushAttrib( GL_TEXTURE_BIT );
	glEnable(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Back
	glBindTexture(GL_TEXTURE_2D, m_TxtID[BACK_ID]);
	glBegin(GL_QUADS);		
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z()); 
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glEnd();

	// Front
	glBindTexture(GL_TEXTURE_2D, m_TxtID[FRONT_ID]);
	glBegin(GL_QUADS);	
	glNormal3f( 0.0f, 0.0f, -1.0f );
	glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glNormal3f( 0.0f, 0.0f, -1.0f );
	glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glNormal3f( 0.0f, 0.0f, -1.0f );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z()); 
	glNormal3f( 0.0f, 0.0f, -1.0f );
	glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glEnd();

	// Bottom
	glBindTexture(GL_TEXTURE_2D, m_TxtID[BOTTOM_ID]);
	glBegin(GL_QUADS);		
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z()); 
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glEnd();

	// TOP 
	glBindTexture(GL_TEXTURE_2D, m_TxtID[TOP_ID]);
	glBegin(GL_QUADS);		
	glNormal3f( 0.0f, -1.0f, 0.0f );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glNormal3f( 0.0f, -1.0f, 0.0f );
	glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z()); 
	glNormal3f( 0.0f, -1.0f, 0.0f );
	glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glNormal3f( 0.0f, -1.0f, 0.0f );
	glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glEnd();

	// Left
	glBindTexture(GL_TEXTURE_2D,m_TxtID[LEFT_ID]);
	glBegin(GL_QUADS);		
	glNormal3f( 1.0f, 0.0f, 0.0f );
	glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());	
	glNormal3f( 1.0f, 0.0f, 0.0f );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z()); 
	glNormal3f( 1.0f, 0.0f, 0.0f );
	glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glNormal3f( 1.0f, 0.0f, 0.0f );
	glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() - m_vHalfDim.x(),	m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());		
	glEnd();

	// Right
	glBindTexture(GL_TEXTURE_2D, m_TxtID[RIGHT_ID]);
	glBegin(GL_QUADS);		
	glNormal3f( -1.0f, 0.0f, 0.0f );
	glTexCoord2f(0.0f, 0.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(), m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glNormal3f( -1.0f, 0.0f, 0.0f );
	glTexCoord2f(1.0f, 0.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(), m_vPos.y() - m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z());
	glNormal3f( -1.0f, 0.0f, 0.0f );
	glTexCoord2f(1.0f, 1.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(), m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() + m_vHalfDim.z()); 
	glNormal3f( -1.0f, 0.0f, 0.0f );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(m_vPos.x() + m_vHalfDim.x(), m_vPos.y() + m_vHalfDim.y(),	m_vPos.z() - m_vHalfDim.z());
	glEnd();

	glDisable(GL_TEXTURE_2D);	
	glPopAttrib();
}

bool CSkyBox::Init()
{
	Config cfg("Config.txt");
	std::string path;

	cfg.loadBlock("SkyBox");
	
	cfg.getString("backBmp", path);
	m_TxtID[BACK_ID]	= LoadTexture(path.c_str());
	
	cfg.getString("frontBmp", path);
	m_TxtID[FRONT_ID] = LoadTexture(path.c_str());
	
	cfg.getString("bottomBmp", path);
	m_TxtID[BOTTOM_ID] = LoadTexture(path.c_str());

	cfg.getString("topBmp", path);
	m_TxtID[TOP_ID]	= LoadTexture(path.c_str());

	cfg.getString("leftBmp", path);
	m_TxtID[LEFT_ID] = LoadTexture(path.c_str());

	cfg.getString("rightBmp", path);
	m_TxtID[RIGHT_ID] = LoadTexture(path.c_str());

	//cfg.getFloat("position", tem, 3);
	//m_vPos.setValue(tem[0], tem[1], tem[2]);

	m_vHalfDim.setValue(500, 500, 500);

	return true;
}
