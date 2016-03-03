#pragma once

#include <deque>
#include "FMutex.h"

template<typename T>
class Fifo
{
private:
	FMutex _mtx;
	std::deque<T> _que;                                                                                 

public:
	Fifo();
	~Fifo();
	//ajoute un �l�ment � la fin
	void push(T element);
	//r�cup�re le premier �l�ment
	T pull();
	//renvoi la taille de la fifo
	int getSize();
};

