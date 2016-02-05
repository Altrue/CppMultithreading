#pragma once

#include <iostream>

class Logger
{
private:
	void insertLog(std::string pContent, int pLevel);
	int _logLevel;

public:
	Logger();
	~Logger();

	void logInfo(std::string pContent);
	void logWarn(std::string pContent);
	void logAlert(std::string pContent);
	void logError(std::string pContent);
	void setLogLevel(int pLevel);
};

