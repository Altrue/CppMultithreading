#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::insertLog(string pContent)
{
	ofstream fichier(this->_fileName, ios::out | ios::app);  // ouverture en écriture avec effacement du fichier ouvert
	if (fichier){
		fichier << pContent << endl;
		fichier.close();
	}
}

string Logger::getTimeStamp()
{
	CDateTime date;
	std::stringstream out;
	date.Now();
	out << date.m_wYear << "/" << date.m_wMonth << "/" << date.m_wDay << " " << date.m_wHour << ":" << date.m_wMinute << ":" << date.m_wSecond;
	return out.str();
}

void Logger::newMessage(int pLevel, string message)
{
	this->_mtx.lock();

	string messageLog;

	messageLog = messageLog + this->getTimeStamp();

	if (pLevel == this->LEVEL_ALERT) {
		messageLog = messageLog + " ALERT : ";
	}
	else if (pLevel == this->LEVEL_ERROR) {
		messageLog = messageLog + " ERROR : ";
	}
	else if (pLevel == this->LEVEL_INFO) {
		messageLog = messageLog + " INFO : ";
	}
	else if (pLevel == this->LEVEL_WARN) {
		messageLog = messageLog + " WARNING : ";
	}

	messageLog = messageLog + message;

	this->insertLog(messageLog);

	this->_mtx.unlock();
}
