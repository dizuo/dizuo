//-----------------------------------------------------------------------------
// File: CTexture.cpp
//
// Desc: Used to load the targa files... uncompressed only at present... You can
//		 either get an id for use with opengl:
//					Create(char* filename, int wrap)
//			or 
//		 just load the targa file for the data (as in a heightmap):
//					int CTexture::LoadTGA(char * filename)				
//
// Source:   http://walterreid.com
//           walter@walterreid.com
//
// Copyright (c) 2000 WalterReid. All rights reserved.
// For educational use only. Any commercial re-use only by written license
//-----------------------------------------------------------------------------
#include "CTexture.h"
#include <cstdio>

CTexture::CTexture()
{
	Data = NULL;
	Width = 0;
	Height = 0;
}


CTexture::~CTexture()
{
    delete Data;
}

GLuint CTexture::Create(const char* filename, int wrap)
{
	m_sName = filename;

    GLuint bpp; //hold bpp, and the actual texture
    
    if (LoadTGA(filename) == 0) 
	{
		return 0; 
	}
    
    if (BytesPerPixel == 4)
	{
		bpp = GL_RGBA;
    }
	else if (BytesPerPixel == 3)
	{
		bpp = GL_RGB;
    }
	else if (BytesPerPixel == 1)
	{
		bpp = GL_ALPHA;
    }
	else
	{
		return 0;
    }

    glGenTextures(1, &m_iTexID); //generate a texture
    glBindTexture(GL_TEXTURE_2D, m_iTexID); //make current texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap); //set if it wraps or clamps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //set texture to mip map
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, Width, Height, bpp, GL_UNSIGNED_BYTE, Data); //generate mipmaps
    
	return m_iTexID;
}

int CTexture::LoadTGA(const char * filename)
{
    unsigned char rgb[12] = {0,0,2,0,0,0,0,0,0,0,0,0};
	unsigned char g[12]   = {0,0,3,0,0,0,0,0,0,0,0,0};
    unsigned char actualheader[12], dataheader[6];
	int imagesize; 
    FILE* file;
    
	file = fopen(filename, "rb");  //open file in binary
    fread(actualheader, 12, 1, file); //read in id header
	

    if (memcmp(actualheader, rgb, 12) != 0 && memcmp(actualheader, g, 12) != 0)
	{
		return 0;
	}
    									
	fread(dataheader, 6, 1, file); //read in header with the image info
    
    BytesPerPixel = dataheader[4] / 8; //get bytes pp
    Width = (dataheader[1] * 256 + dataheader[0]); //get image width
    Height = (dataheader[3] * 256 + dataheader[2]); //get image height


    imagesize = Width * Height * (int)BytesPerPixel; //find out the image size
	
	Data = new unsigned char[imagesize]; //allocate some memory for the image data
		
	fread(Data, 1, imagesize, file); //read in that data

    if (BytesPerPixel == 1) //if greyscale no need to swap
    {
		
	}
	else
	{
		unsigned char temp; //temp variable

		for(int i = 0; i < imagesize ; i+= BytesPerPixel) //make a loop			
		{															
			temp = Data[i]; //swap r b around 		 					
			Data[i] = Data[i + 2];		
			Data[i + 2] = temp;
		}
	}

	fclose(file); //close file
															
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//CTextureManger:
