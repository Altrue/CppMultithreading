#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Observer.h"

class AgentThread: public Observable
{
private:
	std::string t_hash; // hash à trouver
	~AgentThread();

	int returnCode = -1;
	// -1 = Code de retour par défaut qui ne veut rien dire
	// 0 = Thread kill
	// 1 = Mot de passe trouve !

	std::string password = ""; // Vide si on a pas trouve le mot de passe final

public:
	AgentThread(std::string t_hash);
	
	void notifySuccess();
	void killAgent();

};

