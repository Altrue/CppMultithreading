#pragma once

#include <mutex>

class FMutex
{
private:
	std::mutex _mtx;
public:
	FMutex();
	~FMutex();
	//pose un vérou
	void lock();
	//enlève le vérou
	void unlock();
};

