#pragma once

#include <iostream>
#include <string>


#define _CException(message, errorCode)			CException(message, errorCode, __FILE__, __LINE__, __FUNCTION__)


class CException {
protected:
	std::string m_message;
	std::string m_file;
	std::string m_function;
	std::string m_type;
	unsigned int m_line;
	unsigned int m_errorCode;

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	static const std::string CLASS_TYPE;

public:
	CException( const std::string &p_message,
				const unsigned int p_errorCode,
				const std::string &p_file,
				const unsigned int p_line,
				const std::string &p_function );
	virtual ~CException();

	virtual std::string GetFaultLocation() const;
	virtual unsigned int GetErrorCode() const;
	virtual std::string GetErrorMessage() const;
	virtual std::string GetType() const;
};
