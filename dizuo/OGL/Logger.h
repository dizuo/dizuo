/*************************************************************/
/**					Filename: LOGGER.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  31/05/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Create.								**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef __LOGGER_H
#define __LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <fstream>

//#ifdef _DEBUG
//#   define _CLEAR_LOG {Logger log;}
//#   define _LOG(str) {Logger log(str);}
//#else
//#   define _CLEAR_LOG
//#   define _LOG(str)
//#endif

#   define _CLEAR_LOG {Logger log;}
#   define _LOG(str)  {Logger log(str);}


class Logger
{
public:
	/**
	* Constructor
	*/
	Logger();
	Logger( const std::string& entry );

private:
	static const char *LOG_FILE;

}; // end of Logger class

class BadConversion : public std::runtime_error { 
public:
	BadConversion( const std::string& s )
		: std::runtime_error(s)
	{}
};

template<typename T>
inline std::string toStr( T x )
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("stringify(double)");
	return o.str();
}

#endif /*__LOGGER_H*/