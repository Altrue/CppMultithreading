#pragma once

#include <fstream>
#include <stdarg.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <string>

#define INFO  " Info : "
#define WARN  " Warn : "
#define ERROR " Error: "
#define ALERT " Alert : "

using namespace std;

class Logger
{
	private:
		//Mutex mLock;
		ofstream mStream;
		void insertLog(char* pContent, int pLevel);
		int _logLevel;
		char* _fileName;
		char* getTimeStamp();
		std::vector<std::string> _aMessages;

	public:
		Logger();
		~Logger();

		void logInfo(char* pContent);
		void logWarn(char* pContent);
		void logAlert(char* pContent);
		void logError(char* pContent);
		void setLogLevel(int pLevel);
};

