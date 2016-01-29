#pragma once

#include <deque>
#include "PasswordChunk.h"

class Fifo
{
private:
	std::deque<CPasswordChunk> que;

public:
	Fifo();
	~Fifo();
};

