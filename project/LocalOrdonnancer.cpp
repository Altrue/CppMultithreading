#include "LocalOrdonnancer.h"

LocalOrdonnancer::LocalOrdonnancer(Context *_context)
{
	createAgents(_context);
}


LocalOrdonnancer::~LocalOrdonnancer()
{

}

void LocalOrdonnancer::createAgents(Context *_context)
{
	for (int n = 0; n < this->coreCount; n++) {
		AgentThread* thread = new AgentThread(_context);
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

void LocalOrdonnancer::update(int returnCode, std::string returnPassword)
{
	// Mis à jour car un sujet a une info à faire parvenir
	if (returnCode == 0) {
		std::cout << "L'observateur a ete notifie de la mort d'un AgentThread" << std::endl;
	}
	else if (returnCode == 1) {
		std::cout << "L'observateur sait que le mot de passe a ete trouve : " << returnPassword << std::endl;
		this->putDownAgents(); // On arrête donc les agents.
		// TODO : Transmettre la bonne nouvelle (et returnPassword) à l'ordonanceur global
	}
	else if (returnCode == -1) {
		std::cout << "L'observateur a ete notifie d'un message mais le dev a oublie de changer le returnCode donc on ne sait pas ce que c'est..." << std::endl;
	}
	else {
		std::cout << "L'observateur a ete notifie d'un message mais le dev a invente un nouveau returnCode qui n'existe pas..." << std::endl;
	}
	
}


