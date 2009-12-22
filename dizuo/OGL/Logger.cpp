#include "Logger.h"

const char *Logger::LOG_FILE = "logfile.txt";

Logger::Logger() 
{
	std::ofstream log( LOG_FILE );
}

Logger::Logger( const std::string& entry ) 
{
	std::ofstream log( LOG_FILE, std::ios_base::app );

	if( log.is_open() )
		log << entry << std::endl;

	std::cout << entry << std::endl;
}