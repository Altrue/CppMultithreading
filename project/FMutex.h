#pragma once

#include <mutex>

class FMutex
{
private:
	std::mutex _mtx;
public:
	FMutex();
	~FMutex();
	//pose un v�rou
	void lock();
	//enl�ve le v�rou
	void unlock();
};

