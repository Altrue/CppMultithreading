#include "AgentThread.h"
#include <iostream>


AgentThread::AgentThread()
{
	std::cout << "Nouveau AgentThread cree !" << std::endl;
}


void AgentThread::killAgent()
{
	// TODO : On dit à l'agent que ça suffit maintenant faut arrêter de bosser.
	std::cout << "AgentThread decede !" << std::endl;

	this->returnCode = 1;
	// Et on notifie l'observateur
	notify();
}


AgentThread::~AgentThread()
{

}
