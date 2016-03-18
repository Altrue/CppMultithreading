#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Observer.h"
#include "Context.h"

class AgentThread: public Observable
{
private:
	pthread_t thread;
	~AgentThread();

	
	// -1 = Code de retour par d�faut qui ne veut rien dire
	// 0 = Thread kill
	// 1 = Etat de l'avancement (pour la sauvegarde)
	// 2 = Mot de passe trouve !

	

public:
	AgentThread(Context *contexte);

	Context *contexte; // Toutes les infos � conna�tre.
	int returnCode = -1;
	std::string password = "";
	
	void updateLastPassword();
	void notifySuccess();
	void killAgent();
	int run(AgentThread *agent);

};

