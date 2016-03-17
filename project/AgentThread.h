#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Observer.h"

class AgentThread: public Observable
{
private:
	std::string t_hash;
	~AgentThread();

public:
	AgentThread();
	int returnCode = 0;
	void killAgent();
};

