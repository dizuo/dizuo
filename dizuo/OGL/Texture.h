//
//	texture.h - header file
//
//	David Henry - tfc_duke@hotmail.com
//

//-----------------------------------------------------------//
/**   History:                                              **/
/**	  01/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	add logger.							**/
/**					All Rights Reserved.					**/
//-----------------------------------------------------------//
#ifndef		__TEXTURE_H
#define		__TEXTURE_H

#include	"Singleton.hpp"
#include	<list>
#include	<string>

#include	<gl/glew.h>
#include	<GL/glut.h>
#include	<gl/GLAux.h>

#pragma comment(lib, "openGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glew32.lib")
// ==============================================
// CTexture - texture class object.
// ==============================================

class CTexture
{
public:
	// constructors/destructor
	CTexture( void ) { }
	CTexture( const char *texname ) { m_name = texname; }
	CTexture( unsigned int texid, const char *texname )	{ m_id = texid; m_name = texname; }
	~CTexture( void ) { glDeleteTextures( 1, &m_id ); m_name.erase(); }

	unsigned int	GetTexId( void ) { return m_id; }
	const char		*GetName( void ) { return m_name.c_str(); }


public:
	// members variables
	unsigned int	m_id;			// texture id
	std::string		m_name;			// texture name
};



// ==============================================
// CTextureManager - OpenGL texture manager.
// ==============================================

class CTextureManager : public Singleton<CTextureManager>
{
	friend class Singleton<CTextureManager>;
protected:
	// constructor/destructor
	CTextureManager( void ) { Initialize(); }
	virtual ~CTextureManager( void ) { CleanAllTextures(); }

public:
	// functions
	void			Initialize( void );
	unsigned int	LoadTexture( const char *filename );
	void			DeleteTexture( unsigned int id );
	void			CleanAllTextures( void );
	
	// my addition...
	unsigned int	BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);
private:
	// linked texture list
	typedef std::list<CTexture *>	TextureList;
	typedef TextureList::iterator	TListItor;

	TextureList						m_texlist;
};



// global function using the texture manager
inline unsigned int LoadTexture( const char *filename )
{
	return CTextureManager::GetInstance()->LoadTexture( filename );
}


#endif	// __TEXTURE_H
