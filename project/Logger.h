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
#include "FMutex.h"
#include "CDateTime.h"
#include <sstream>
#include <string>

class Logger
{
	private:
		string _fileName = "dhc.log";
		FMutex _mtx;
		string getTimeStamp();
		void insertLog(string pContent);

	public:
		const int LEVEL_INFO = 0;
		const int LEVEL_ERROR = 3;
		const int LEVEL_ALERT = 2;
		const int LEVEL_WARN = 1;
		Logger();
		~Logger();
		void newMessage(int pLevel, string message);
};

