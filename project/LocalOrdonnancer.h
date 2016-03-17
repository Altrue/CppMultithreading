#pragma once

#include "Fifo.h"
#include "PasswordChunk.h"
#include "CUtil.h"
#include "Observer.h"
#include "AgentThread.h"

class LocalOrdonnancer: public Observer
{
	private:
		std::deque<CPasswordChunk> fifo;
		const int coreCount = CUtil::GetCpuCoreCount();
		std::vector<AgentThread*> vectorAgents;
		void createAgents(std::string t_hash);

		AgentThread* agentInfo_;

	public:
		LocalOrdonnancer(std::string t_hash);
		~LocalOrdonnancer();

		void putDownAgents();
		void update(int returnCode, std::string returnPassword);
};

