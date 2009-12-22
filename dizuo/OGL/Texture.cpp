//
//	texture.cpp - source file
//
//	David Henry - tfc_duke@hotmail.com
//
#include	"Texture.h"
#include	"bmp.h"
#include	"tga.h"
#include	"pcx.h"
#include	"Logger.h"

#include	<iostream>

// ----------------------------------------------
// Initialize() - create a checker for the default
// texture.
// ----------------------------------------------

void CTextureManager::Initialize( void )
{
	// this is the first texture loaded. If a texture
	// can't be loaded, we use this instead

	// initialize only once!
	// if( (*m_texlist.begin()) == (*m_texlist.end()) )
	if( m_texlist.empty() )
	{
		CTexture *tex = new CTexture( "default" );

		// create and initialize the texture
		glGenTextures( 1, &tex->m_id );
		glBindTexture( GL_TEXTURE_2D, tex->m_id );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		m_texlist.push_back( tex );

		// create a checker for the default texture
		int i, j, c;				// temporary variable
		unsigned char *checker;		// texture data

		checker = new unsigned char[ 64 * 64 * 4 ];

		for( i = 0; i < 64; i++ )
		{
			for( j = 0; j < 64; j++ )
			{
				c = ( !(i & 8) ^ !(j & 8)) * 255;

				checker[ (i * 256) + (j * 4) + 0 ] = (unsigned char)c;
				checker[ (i * 256) + (j * 4) + 1 ] = (unsigned char)c;
				checker[ (i * 256) + (j * 4) + 2 ] = (unsigned char)c;
				checker[ (i * 256) + (j * 4) + 3 ] = (unsigned char)255;

			}
		}
		glTexImage2D( GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker );

		delete [] checker;
	}
}



// ----------------------------------------------
// LoadTexture() - load a texture from file. Verify
// if it has not already been loaded before. If yes,
// return stored texture's id, otherwise, load it
// and return the new texture id generated.
// ----------------------------------------------

unsigned int CTextureManager::LoadTexture( const char *filename )
{
	unsigned int	id = 0;
	unsigned char	*texels = 0;
	int				width, height;
	int				success;
	
	_AUX_RGBImageRec* Image = NULL;
	
	for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
	{
		if( strcmp( (*itor)->GetName(), filename ) == 0 )
			return (*itor)->GetTexId();
	}


	if( strstr( filename, ".bmp" ) || strstr( filename, ".BMP" ) )
	{
		_LOG( "Add Texture .BMP file from " + std::string(filename) );
		//success = LoadFileBMP( filename, &texels, &width, &height, true );
		//使用glaux里面的函数

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1,&id); 
		glBindTexture( GL_TEXTURE_2D, id);
		Image = auxDIBImageLoadA( (const char*) filename );
		int Width = Image->sizeX;
		int Height = Image->sizeY;
		gluBuild2DMipmaps(	GL_TEXTURE_2D, 
			3, 
			Image->sizeX,
			Image->sizeY,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			Image->data );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		delete Image;
		return id;
	}

	if( strstr( filename, ".tga" ) || strstr( filename, ".TGA" ) ) {
		_LOG( "Add Texture .TGA file from " + std::string(filename) );
		success = LoadFileTGA( filename, &texels, &width, &height, true );
	}

	if( strstr( filename, ".pcx" ) || strstr( filename, ".PCX" ) )
	{
		_LOG( "Add Texture .PCX file from " + std::string(filename) );
		success = LoadFilePCX( filename, &texels, &width, &height, true );
	}

	if( success > 0)
	{
		// create and initialize new texture
		glGenTextures( 1, &id );
		glBindTexture( GL_TEXTURE_2D, id );
	
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texels );

		// create a new CTexture object and push it at the end of the linked list
		CTexture *tex = new CTexture( id, filename );
		m_texlist.push_back( tex );
	}
	else
	{
		// can't load the texture, use default texture
		id = (*m_texlist.begin())->GetTexId();
	}


	if( texels )
		delete [] texels;


	return id;
}

// ----------------------------------------------
// Build a texture from the color value
// ----------------------------------------------
unsigned int CTextureManager::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits
	unsigned int id;

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &id);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);

	return id;
}


// ----------------------------------------------
// DeleteTexture() - delete texture.
// ----------------------------------------------
void CTextureManager::DeleteTexture( unsigned int id )
{
	for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
	{
		if( (*itor)->GetTexId() == id )
		{
			delete (*itor);
			itor = m_texlist.erase( itor );
		}
	}
}


// ----------------------------------------------
// CleanAllTextures() - delete all textures stored
// in the linked list.
// ----------------------------------------------

void CTextureManager::CleanAllTextures( void )
{
	for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
	{
		delete (*itor);
		//itor = m_texlist.erase( itor );
	}
}
