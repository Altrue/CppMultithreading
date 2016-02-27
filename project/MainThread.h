#pragma once

#include <iostream>
#include <string>

class MainThread{
private:
	std::string t_hash;

public:
	MainThread(std::string hash);
	~MainThread();
};

