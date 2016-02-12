#include "FMutex.h"



FMutex::FMutex()
{
}


FMutex::~FMutex()
{
}

void FMutex::lock() {
	this->_mtx.lock();
}

void FMutex::unlock() {
	this->_mtx.unlock();
}
