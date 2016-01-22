#pragma once

#include <iostream>
#include <string>

#include "CException.h"


#define _CSocketIOException(message, errorCode)			CSocketIOException(message, errorCode, __FILE__, __LINE__, __FUNCTION__)


class CSocketIOException : public CException {
public:
	static const std::string CLASS_TYPE;

public:
	CSocketIOException( const std::string &p_message,
						const unsigned int p_errorCode,
						const std::string &p_file,
						const unsigned int p_line,
						const std::string &p_function );
};
