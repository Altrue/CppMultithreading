#include "AgentThread.h"
#include <iostream>


AgentThread::AgentThread(Context *_contexte)
{
	std::cout << "Nouveau AgentThread cree !" << std::endl;
	this->contexte = _contexte;
}


void AgentThread::notifySuccess()
{
	std::cout << "AgentThread a trouve le mot de passe : " << this->password << std::endl;

	this->returnCode = 1;
	notify(this->returnCode, this->password);
}

void AgentThread::killAgent()
{
	// TODO : On dit à l'agent que ça suffit maintenant faut arrêter de bosser.
	std::cout << "AgentThread decede !" << std::endl;

	this->returnCode = 0;
	// Et on notifie l'observateur
	notify(this->returnCode, this->password);
}


AgentThread::~AgentThread()
{

}
