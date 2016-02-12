#pragma once

#include <mutex>

class FMutex
{
private:
	std::mutex _mtx;
public:
	FMutex();
	~FMutex();
	void lock();
	void unlock();
};

