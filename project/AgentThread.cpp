#include "AgentThread.h"
#include <iostream>


AgentThread::AgentThread()
{
	std::cout << "Nouveau AgentThread cree !" << std::endl;
}


void AgentThread::killAgent()
{
	// TODO : On dit � l'agent que �a suffit maintenant faut arr�ter de bosser.
	std::cout << "AgentThread decede !" << std::endl;

	this->returnCode = 1;
	// Et on notifie l'observateur
	notify();
}


AgentThread::~AgentThread()
{

}
