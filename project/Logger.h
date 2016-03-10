#pragma once

#include "stdafx.h"
#include <fstream>
#include <stdarg.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <string>

#define ERROR " Error : "
#define ALERT " Alert : "
#define WARN  " Warn : "
#define INFO  " Info : "
#define DETAIL " Detail : "

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
		// Récupération de la date à partir de la class du prof
		std::string getTimeStamp();
		// Insertion dans le fichier
		void insertLog(std::string pContent);

	public:
		// Les niveaux de log
		void initialize(int level);
		const int LEVEL_ERROR = 0;
		const int LEVEL_ALERT = 1;
		const int LEVEL_WARN = 2;
		const int LEVEL_INFO = 3;
		const int LEVEL_DEBUG = 4;
		int verboseLevel;
		~Logger();
		// Fonction utilisée pour créer les logs (thread-safe)
		void newMessage(int pLevel, std::string message);

};
