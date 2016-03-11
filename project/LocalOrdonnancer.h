#pragma once

#include "Fifo.h"
#include "PasswordChunk.h"
#include "AgentThread.h"
#include "CUtil.h"
#include <vector>


class LocalOrdonnancer
{
	private:
		std::deque<CPasswordChunk> fifo;
		const int coreCount = CUtil::GetCpuCoreCount();
		std::vector<AgentThread*> vectorAgents;
		void createAgents();

	public:
		LocalOrdonnancer();
		~LocalOrdonnancer();

		void putDownAgents();
};

