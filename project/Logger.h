#pragma once

#include "stdafx.h"
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

#include "FMutex.h"
#include "CDateTime.h"
#include <sstream>
#include <string>
#include "singleton.h"

class Logger
{
	IMPLEMENT_SINGLETON(Logger);

	private:
		std::string _fileName = "dhc.log";
		FMutex _mtx;
		// R�cup�ration de la date � partir de la class du prof
		std::string getTimeStamp();
		// Insertion dans le fichier
		void insertLog(std::string pContent);

	public:
		// Les niveaux de log
		const int LEVEL_INFO = 0;
		const int LEVEL_ERROR = 3;
		const int LEVEL_ALERT = 2;
		const int LEVEL_WARN = 1;
		~Logger();
		// Fonction utilis�e pour cr�er les logs (thread-safe)
		void newMessage(int pLevel, std::string message);

};
