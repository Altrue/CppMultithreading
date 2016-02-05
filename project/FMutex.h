#pragma once

#include <mutex>

class FMutex
{
private:
	std::mutex mtx;
public:
	FMutex();
	~FMutex();
	void lock();
	void unlock();
};

