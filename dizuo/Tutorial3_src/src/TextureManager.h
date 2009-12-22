#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "Texture.h"
#include <string>
#include <map>

// The texture manager avoid a same texture to be loaded multiple
// times. It keeps a map containing all the already loaded textures.
class CTextureManager
{
public:
	// Loads a texture specified by its filename. If the texture is not
	// loaded already, the texture manager will load it, store it and 
	// return it. Otherwise it simply returns the existing one.
	CTexture* GetTexture(const std::string& strTextName);
	// Release the texture specified by its filename. Returns true if 
	// the texture was found, otherwise false.
	bool ReleaseTexture(const std::string& strTextName);

	// Returns the single instance of the texture manager.
	// The manager is implemented as a singleton.
	static CTextureManager* GetInstance();

protected:
	// Both constructor and destructor are protected to make
	// it impossible to create an instance directly.
	CTextureManager();
	~CTextureManager();

private:
	typedef std::map<std::string,CTexture*> TTextureMap;
	// The map of already loaded textures. There are indexed
	// using their filename.
	TTextureMap m_Textures;
};

#endif  // _TEXTUREMANAGER_H_