#pragma once

#include <string>

class AgentThread
{
private:
	std::string t_hash;
	~AgentThread();

public:
	AgentThread();
	void killAgent();
};

