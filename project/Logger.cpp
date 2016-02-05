#include "Logger.h"

void Logger::insertLog(std::string pContent, int pLevel)
{
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::logInfo(std::string pContent)
{
	insertLog(pContent, 4);
}

void Logger::logWarn(std::string pContent)
{
	insertLog(pContent, 3);
}

void Logger::logAlert(std::string pContent)
{
	insertLog(pContent, 2);
}

void Logger::logError(std::string pContent)
{
	insertLog(pContent, 1);
}

void Logger::setLogLevel(int pLevel)
{
	if (pLevel > 0) {
		_logLevel = pLevel;
	}
}
