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
		this->vectorAgents.push_back(thread);
	}
}

void LocalOrdonnancer::putDownAgents()
{
	for (int n = 0; n < this->coreCount; n++) {
		this->vectorAgents[n]->killAgent();
		this->vectorAgents.pop_back();
	}
}


