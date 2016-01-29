#pragma once

#include <mutex>

class StdMtx
{
private:
	std::mutex mtx;
public:
	StdMtx();
	~StdMtx();
	void lock();
	void unlock();
};

