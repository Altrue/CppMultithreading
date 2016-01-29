#include "Mutex.h"



StdMtx::StdMtx()
{
}


StdMtx::~StdMtx()
{
}

void StdMtx::lock() {
	mtx.lock();
}

void StdMtx::unlock() {
	mtx.unlock();
}
