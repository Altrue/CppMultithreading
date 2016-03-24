#include "Logger.h"

// Public
Logger::~Logger()
{

}

void Logger::initialize(int level) {
	this->verboseLevel = level;
}

void Logger::insertLog(std::string pContent)
{
	std::ofstream fichier(this->_fileName, std::ios::out | std::ios::app);  // ouverture en écriture avec effacement du fichier ouvert
	if (fichier){
		fichier << pContent << std::endl;
		fichier.close();
	}
}

std::string Logger::getTimeStamp()
{
	CDateTime date;
	std::stringstream out;
	date.Now();
	
	std::string nbMois = std::to_string(date.m_wMonth);
	std::string nbJours = std::to_string(date.m_wDay);
	std::string nbHeures = std::to_string(date.m_wHour);
	std::string nbMinutes = std::to_string(date.m_wMinute);
	std::string nbSecondes = std::to_string(date.m_wSecond);
	// Super joli le décalage d'un caractère à chaque fois

	// Normalisation de la date (exemple 3 -> 03)
	if (nbMois.length() < 2) { nbMois = "0" + nbMois; }
	if (nbJours.length() < 2) { nbJours = "0" + nbJours; }
	if (nbHeures.length() < 2) { nbHeures = "0" + nbHeures; }
	if (nbMinutes.length() < 2) { nbMinutes = "0" + nbMinutes; }
	if (nbSecondes.length() < 2) { nbSecondes = "0" + nbSecondes; }

	out << date.m_wYear << "/" << nbMois << "/" << nbJours << " " << nbHeures << ":" << nbMinutes << ":" << nbSecondes;
	return out.str();
}

void Logger::newMessage(int pLevel, std::string message)
{
	this->_mtx.lock();
	if (pLevel <= this->verboseLevel) {
		std::string messageLog;

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
		else if (pLevel == this->LEVEL_DEBUG) {
			messageLog = messageLog + " DEBUG : ";
		}
		else if (pLevel == this->LEVEL_WARN) {
			messageLog = messageLog + " WARNING : ";
		}

		messageLog = messageLog + message;
		std::cout << messageLog << std::endl;

		this->insertLog(messageLog);
	}

	this->_mtx.unlock();
}
