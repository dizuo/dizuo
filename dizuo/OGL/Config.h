/*************************************************************/
/**					Filename: CONFIG.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  31/05/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Create.								**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>

class Config
{
private:
	// The file stream
	std::ifstream m_File;

	// The key items
	std::map < std::string, std::string > m_Items;

public:
	/**
	* Constructors
	*/
	Config();
	Config( const char *cfg_filename );

	/**
	* Opens the stream and returns whether it was successfull
	*/
	bool open( const char *cfg_filename );

	/**
	* Closes the stream
	*/
	void close();

	/**
	* Loads a block of key-value paired data under the 
	* given block title
	*/
	int loadBlock( const std::string &sTitle );

	/**
	* An simple method if we want a string value
	*/
	void getString( const std::string &sKey, std::string &sOut );

	/**
	* Reads integers
	*/
	void getInt( const std::string &sKey, 
		int *out, 
		const int nCount = 1 );

	/**
	* Reads booleans
	*/
	void getBool( const std::string &sKey, 
		bool *out, 
		const int nCount = 1 );


	/**
	* Reads floats
	*/
	void getFloat( const std::string &sKey, 
		float *out, 
		const int nCount = 1 );

	/**
	* Reads doubles
	*/
	void getDouble( const std::string &sKey, 
		double *out, 
		const int nCount = 1 );

	/**
	* Reads chars
	*/
	void getChar( const std::string &sKey, 
		char *out, 
		const int nCount = 1 );

	static std::string path;
	static float temp[3];
private:
	/**
	* Saves any number of values of any kind from a key-value
	* pair
	*/
	template<typename T>
	void _get( const std::string &sKey, T *out, const int nCount );

	/**
	* Returns the file pointer to the beginning of the file
	*/
	void _reset();

}; // end of class Config

// ------------------------------------------------------------------------
template<typename T>
void Config::_get( const std::string &sKey, T *out, const int nCount )
{
	// Create a string stream to easily read the values
	// from out data string
	std::istringstream instream;
	instream.clear();
	instream.str( m_Items[sKey] );

	// dummy character to read delimiter
	char d;
	// 将data部分存放到out数组中，忽略中间的,分隔符.
	for( int i=0; i<nCount; ++i ) {
		instream >> out[i] >> d;	
	}

} // end of _get

#endif /* __CONFIG_H */

/*
 *Config文件格式说明：
 *	不同的类型以符号:  "." 来区分
 *	每一行有两项:name data	
 */