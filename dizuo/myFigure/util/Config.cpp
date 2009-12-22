#include "Config.h"

std::string Config::path;
float Config::temp[3];

Config::Config() {}
Config::Config( const char *cfg_filename ) {
	open( cfg_filename );
}

// ------------------------------------------------------------------------
bool Config::open( const char *cfg_filename )
{
	m_File.open( cfg_filename );
	return m_File.is_open();
}

// ------------------------------------------------------------------------
void Config::close() { m_File.close(); }

// ------------------------------------------------------------------------
int Config::loadBlock( const std::string &sTitle )
{
	// Return if file is not open
	if( !m_File.is_open() ) return -1;

	std::string sCmpTitle;
	while( m_File >> sCmpTitle ) 
	{
		// Seach for a keyword with a dot(.) at the start
		// That is how a block title is represented
		if( (std::string(".")+sTitle) == sCmpTitle )
		{
			// Now save its data in std::map structure
			std::string sKey, sValue;
			//循环每次处理一行,见Config文件格式.
			while( m_File >> sKey >> sValue )
			{
				// Quit when we reach another data block
				if( sKey[0] == '.' || sKey[0] == ' ' ) break;

				m_Items[ sKey ] = sValue;
			}

			// Reset file before exiting
			_reset();

			// Return 1 for finding the block
			return 1;

		} // end if( )

	} // end while( )

	// Return 0 for finding nothing
	return 0;

} // end _get()

// ------------------------------------------------------------------------
void Config::getInt( const std::string &sKey, 
					int *out, 
					const int nCount )
{
	_get<int>( sKey, out, nCount );
}

// ------------------------------------------------------------------------
void Config::getBool( const std::string &sKey, 
					 bool *out, 
					 const int nCount )
{
	_get<bool>( sKey, out, nCount );
}

// ------------------------------------------------------------------------
void Config::getChar( const std::string &sKey, 
					 char *out, 
					 const int nCount )
{
	_get<char>( sKey, out, nCount );
}

// ------------------------------------------------------------------------
void Config::getFloat( const std::string &sKey, 
					  float *out, 
					  const int nCount )
{
	_get<float>( sKey, out, nCount );
}

// ------------------------------------------------------------------------
void Config::getDouble( const std::string &sKey, 
					   double *out, 
					   const int nCount )
{
	_get<double>( sKey, out, nCount );
}

// ------------------------------------------------------------------------
void Config::getString( const std::string &sKey, std::string &sOut ) {
	sOut = m_Items[ sKey ];
}

// ------------------------------------------------------------------------
void Config::_reset()
{
	m_File.clear();
	m_File.seekg( 0, std::ios_base::beg );
}
