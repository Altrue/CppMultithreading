#include "Context.h"



void Context::fillContext(Logger *_logger, std::string _hash, std::string _algo, std::string _alphabet, int _chunkSize, std::string _masterIpAddress, bool _forceNew, CPasswordChunk *_repriseChunk)
{
	this->logger = _logger;
	this->hash = _hash;
	this->algo = _algo;
	this->alphabet = _alphabet;
	this->chunkSize = _chunkSize;
	this->masterIpAddress = _masterIpAddress;
	this->forceNew = _forceNew;
	this->repriseChunk = _repriseChunk;
}

void Context::plugFifo(Fifo<CPasswordChunk> *_fifo)
{
	this->fifo = _fifo;
}

Context::Context()
{
	
}

Context::~Context()
{

}
