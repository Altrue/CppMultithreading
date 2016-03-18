#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Observer.h"
#include "Context.h"

class AgentThread: public Observable
{
private:
	Context *contexte; // Toutes les infos à connaître.
	~AgentThread();

	int returnCode = -1;
	// -1 = Code de retour par défaut qui ne veut rien dire
	// 0 = Thread kill
	// 1 = Mot de passe trouve !

	std::string password = ""; // Vide si on a pas trouve le mot de passe final

public:
	AgentThread(Context *contexte);
	
	void notifySuccess();
	void killAgent();

};

