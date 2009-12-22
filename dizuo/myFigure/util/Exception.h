#ifndef EXCEPTION_H
#define EXCEPTION_H

// 消除VC不支持标准异常规范的警告
#pragma warning( disable : 4290 )
#include <string>

class Exception
{
public:
	virtual std::string GetErrorMessage() const
	{
		static std::string sGenetic[] = "The Serious Mistake";
		return sGenetic;
	}
};

class SimpleException : public Exception
{
public:
	SimpleException(std::string sMessage)
		: m_sMessage(sMessage)
	{}
	virtual std::string GetErrorMessage() const { return m_sMessage; }
private:
	std::string m_sMessage;
};

class InitException : public Exception
{

};

class FileException : public Exception
{
public:
	enum Cause{
		Generic,
		OpenFailed,
		WriteFailed,
		BadFormat
	};
	FileException(std::string sMessage, Cause cause = FileException::Generic)
		: m_sMessage(sMessage), m_Cause(cause)
	{}
	
	virtual std::string GetErrorMessage() const { return m_sMessage; };
	Cause				GetCause() const{ return m_Cause; }
private:
	std::string m_sMessage;
	Cause m_Cause;
};

#endif