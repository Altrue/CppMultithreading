#include "FMutex.h"



FMutex::FMutex()
{
}


FMutex::~FMutex()
{
}

void FMutex::lock() {
	mtx.lock();
}

void FMutex::unlock() {
	mtx.unlock();
}
