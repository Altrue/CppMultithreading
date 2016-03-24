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

public:
	AgentThread(Context *contexte);

	Context *contexte; // Toutes les infos à connaître.
	std::string password = "";
	int returnCode = -1;
	// -1 = Code de retour par défaut qui ne veut rien dire
	// 0 = Thread kill
	// 1 = Etat de l'avancement (pour la sauvegarde)
	// 2 = Mot de passe trouve !
	
	
	void updateLastPassword();
	void notifySuccess();
	void killAgent();

};

