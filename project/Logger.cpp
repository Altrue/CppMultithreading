#include "Logger.h"
#include <stdarg.h>

Logger::Logger()
{
	//mutexInit(&mLock, NULL);
	mStream.open(_fileName, fstream::app);
	_aMessages.push_back("");
	_aMessages.push_back(ERROR);
	_aMessages.push_back(ALERT);
	_aMessages.push_back(WARN);
	_aMessages.push_back(INFO);
}

Logger::~Logger()
{
	mStream.close();
	//mutexDestroy(&mLock);
}

void Logger::insertLog(char* pContent, int pLevel)
{
	//mutexLock(&mLock);
	va_list argList;
	char buffer[1024];

	va_start(argList, pContent);
	vsnprintf(buffer, 1024, pContent, argList);
	va_end(argList);

	mStream << getTimeStamp() << _aMessages[pLevel] << buffer << endl;
	//mutexUnlock(&mLock);
}

char * Logger::getTimeStamp()
{
	char*      tString = new char[80];
	time_t     t       = time(0);
	struct tm* today   = localtime(&t);

	strftime(tString, 80, "%d/%m/%Y %H:%M:%S", today);

	return tString;
}

void Logger::logInfo(char* pContent)
{
	insertLog(pContent, 4);
}

void Logger::logWarn(char* pContent)
{
	insertLog(pContent, 3);
}

void Logger::logAlert(char* pContent)
{
	insertLog(pContent, 2);
}

void Logger::logError(char* pContent)
{
	insertLog(pContent, 1);
}

void Logger::setLogLevel(int pLevel)
{
	if (pLevel > 0) {
		_logLevel = pLevel;
	}
}
