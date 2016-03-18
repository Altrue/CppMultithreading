#pragma once

#include "Fifo.h"
#include "PasswordChunk.h"
#include "CUtil.h"
#include "Observer.h"
#include "AgentThread.h"
#include "Context.h"

class LocalOrdonnancer: public Observer
{
	private:
		std::deque<CPasswordChunk> fifo;
		const int coreCount = CUtil::GetCpuCoreCount();
		std::vector<AgentThread*> vectorAgents;
		void createAgents(Context *contexte);

		AgentThread* agentInfo_;

	public:
		LocalOrdonnancer(Context *contexte);
		~LocalOrdonnancer();

		void putDownAgents();
		void update(int returnCode, std::string returnPassword);
};
