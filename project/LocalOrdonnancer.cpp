#include "LocalOrdonnancer.h"

LocalOrdonnancer::LocalOrdonnancer()
{
	createAgents();
}


LocalOrdonnancer::~LocalOrdonnancer()
{

}

void LocalOrdonnancer::createAgents()
{
	for (int n = 0; n < this->coreCount; n++) {
		AgentThread* thread = new AgentThread();
		thread->attach(this);
		std::cout << "AgentThread " << n << " est desormais observe !" << std::endl;
		this->vectorAgents.push_back(thread);
	}
}

void LocalOrdonnancer::putDownAgents()
{
	for (int n = 0; n < this->coreCount; n++) {
		this->vectorAgents[n]->killAgent();
	}

	this->vectorAgents.clear();
}

void LocalOrdonnancer::update()
{
	// Mis à jour car un sujet a changé d'état
	std::cout << "L'observateur a ete notifie de quelque chose." << std::endl;
	// Pour le moment on a aucune info, on regarde juste que ça marche.
}


