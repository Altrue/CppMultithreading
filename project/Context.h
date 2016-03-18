#pragma once

#include <iostream>
#include "Logger.h"
#include "PasswordChunk.h"
#include "Fifo.h"

class Context
{
public:
	Logger *logger;
	std::string hash;
	std::string algo;
	std::string alphabet;
	int chunkSize;
	std::string masterIpAddress;
	bool forceNew;
	CPasswordChunk *repriseChunk;
	Fifo<CPasswordChunk> *fifo;

	Context();
	~Context();

	void fillContext(Logger *logger, std::string hash, std::string algo, std::string alphabet, int chunkSize, std::string masterIpAddress, bool forceNew, CPasswordChunk *repriseChunk);
	void plugFifo(Fifo<CPasswordChunk> *fifo);
};

