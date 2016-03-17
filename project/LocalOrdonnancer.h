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
		void createAgents();

		AgentThread* agentInfo_;

	public:
		LocalOrdonnancer();
		~LocalOrdonnancer();

		void putDownAgents();
		void update();
};

