
#include "TextureManager.h"
#include "Exception.h"
#include <IL/IL.h>

using namespace std;

CTextureManager::CTextureManager() : m_Textures()
{
	// Initialize DevIL
	ilInit();

	// Set the first loaded point to the 
	// upper-left corner.
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilEnable(IL_ORIGIN_SET);
}

CTextureManager::~CTextureManager()
{
	TTextureMap::iterator iter = m_Textures.begin();
	for (iter; iter != m_Textures.end(); iter++)
	{
		if (iter->second)
			delete iter->second;
	}
}

CTextureManager* CTextureManager::GetInstance()
{
	// Returns the unique class instance.
	static CTextureManager Instance;
	return &Instance;
}

CTexture* CTextureManager::GetTexture(const string& strTextName)
{
	// Look in the map if the texture is already loaded.
	TTextureMap::const_iterator iter = m_Textures.find(strTextName);
	if (iter != m_Textures.end())
		return iter->second;

	// If it was not found, try to load it from file. If the load 
	// failed, delete the texture and throw an exception.
	CTexture* pNewText = NULL;
	try
	{
		pNewText = new CTexture(strTextName);
	}
	catch (CException& e)
	{
		delete pNewText;
		throw e;
	}

	// Store the newly loaded texture and return it.
	m_Textures[strTextName] = pNewText;
	return pNewText;
}

bool CTextureManager::ReleaseTexture(const std::string& strTextName)
{
	// Retrieve the texture from the map
	bool bFound = false;
	TTextureMap::iterator iter = m_Textures.find(strTextName);
	if (iter != m_Textures.end())
	{
		// If it was found, we delete it and remove the
		// pointer from the map.
		bFound = true;
		if (iter->second)
			delete iter->second;
		m_Textures.erase(iter);
	}

	return bFound;
}
